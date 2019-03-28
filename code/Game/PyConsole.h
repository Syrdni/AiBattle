#include "IMUI/IMUI.h"
#include "Core/String/String.h"
#include "Core/String/StringBuilder.h"
#include "Core/Containers/Array.h"
#include "Core/Containers/StaticArray.h"
#include "pybind11/pybind11.h"
#include "pybind11/embed.h"
#include "ctype.h"

using namespace Oryol;

namespace PySetup
{
	// initialize python
	inline void Load()
	{
		o_assert(!Py_IsInitialized());
		Py_Initialize();
	}

	// finalize python
	inline void Unload()
	{
		o_assert(Py_IsInitialized());
		Py_Finalize();
	}

	// redirects an output stream
	template<typename T>
	inline void SetOutputStream(const char* stream, T t)
	{
		o_assert(Py_IsInitialized());

		PyObject* out = PySys_GetObject(stream);
		if (out == nullptr || out == Py_None)
		{
			StringBuilder sb;
			sb.Append("import sys\nsys.");
			sb.Append(stream);
			sb.Append(" = type(\"\",(object,),{\"write\":lambda self, txt: None, \"flush\":lambda self: None})()\n");
			PyRun_SimpleString(sb.AsCStr());
			out = PySys_GetObject(stream);
		}

		PyObject_SetAttrString(out, "write", pybind11::cpp_function(std::forward<T>(t)).ptr());
	}
}

/**
	Code based on ExampleAppConsole from ImGui demo at https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp

	@author Philip Stenmark
*/
class PyConsole
{
public:
	void PyConsole::Setup()
	{
		// load python
		PySetup::Load();

		// setup stream redirects
		PySetup::SetOutputStream("stdout", [this](const char* msg)
		{
			strBuilder.Append(msg);
			if (strBuilder.Contains("\n"))
			{
				this->log.Add({ LogType::Message, strBuilder.GetString() });
				strBuilder.Clear();
			}
		});

		PySetup::SetOutputStream("stderr", [this](const char* msg)
		{
			strBuilder.Append(msg);
			if (strBuilder.Contains("\n"))
			{
				this->log.Add({ LogType::Error, strBuilder.GetString() });
				strBuilder.Clear();
			}
		});

		this->historyIndex = -1;
	}

	void PyConsole::Discard()
	{
		this->Clear();
		this->history.Clear();
		PySetup::Unload();
	}

	void PyConsole::Clear()
	{
		log.Clear();
	}

	void PyConsole::Draw()
	{
		if (!ImGui::Begin("Console", nullptr, ImVec2(400, 400), 0.7f, ImGuiWindowFlags_NoScrollbar))
		{
			ImGui::End();
			return;
		}

		ImGui::Separator();
		ImGui::Checkbox("Autoscroll", &this->autoScoll);
		ImVec4 color;
		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
		for (const auto& entry : this->log)
		{ 
			switch (entry.type)
			{
			case LogType::Message:
				color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
				break;
			case LogType::Error:
				color = ImVec4(1.0f, 0.2f, 0.2f, 1.0f);
				break;
			case LogType::Info:
				color = ImVec4(1.0f, 1.0f, 0.2f, 1.0f);
				break;
			}
			ImGui::PushStyleColor(ImGuiCol_Text, color);
			ImGui::TextUnformatted(entry.message.AsCStr());
			ImGui::PopStyleColor();
		}
		// scroll log to bottom
		if(this->autoScoll) ImGui::SetScrollHereY();

		ImGui::EndChild();
		ImGui::Separator();

		bool focus = false;
		if (ImGui::InputText("Input", &inputBuf[0], inputBuf.Size(), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackHistory, &TextEditCallback, (void*)this))
		{
			this->Execute(&inputBuf.MakeSlice(0, inputBuf.Size())[0]);
			inputBuf.Fill(0);
			focus = true;
		}

		ImGui::SetItemDefaultFocus();
		if (focus)
			ImGui::SetKeyboardFocusHere(-1);

		ImGui::End();
	}

	/// execute a built-in command or a python command
	void Execute(const String& cmd, bool hidden = false)
	{
		o_assert_dbg(Py_IsInitialized());

		if (!cmd.IsValid())
			return;

		if (!hidden)
		{
			// add command to log
			StringBuilder sb;
			sb.Append("# ");
			sb.Append(cmd);
			this->log.Add({ LogType::Message, sb.GetString() });

			this->historyIndex = -1;
			for (int i = history.Size() - 1; i >= 0; i--)
			{
				if (history[i] == cmd)
				{
					history.Erase(i);
					break;
				}
			}
			this->history.Add(cmd);
		}

		if (cmd == "clear")
		{
			this->Clear();
		}
		else if (cmd == "help")
		{
			this->Log("Testing the test");
		}
		else if (cmd == "history")
		{
			int first = this->history.Size() - 10;
			for (int i = first > 0 ? first : 0; i < history.Size(); i++)
			{
				this->Log("%3d: %s", i, history[i].AsCStr());
			}
		}
		else
		{
			// interpret python
			PyRun_SimpleString(cmd.AsCStr());
		}
	}

	/// log an info message
	void Log(const char* fmt, ...)
	{
		char buffer[1024];
		va_list args;
		va_start(args, fmt);
		vsnprintf(buffer, sizeof(buffer), fmt, args);
		buffer[sizeof(buffer) - 1] = 0;
		va_end(args);
		this->log.Add({ LogType::Info, buffer });
	}

	static int TextEditCallback(ImGuiInputTextCallbackData* data)
	{
		PyConsole* console = (PyConsole*)data->UserData;
		switch (data->EventFlag)
		{
		case ImGuiInputTextFlags_CallbackHistory:
			const int previous = console->historyIndex;
			if (data->EventKey == ImGuiKey_UpArrow)
			{
				if (console->historyIndex == -1)
				{
					console->historyIndex = console->history.Size() - 1;
				}
				else if (console->historyIndex > 0)
				{
					console->historyIndex--;
				}
			}
			else if (data->EventKey == ImGuiKey_DownArrow)
			{
				if (console->historyIndex != -1)
				{
					if (++console->historyIndex >= console->history.Size())
					{
						console->historyIndex = -1;
					}
				}
			}

			if (previous != console->historyIndex)
			{
				String str = (console->historyIndex >= 0) ? console->history[console->historyIndex] : "";
				data->DeleteChars(0, data->BufTextLen);
				data->InsertChars(0, str.AsCStr());
			}

			break;
		}
		return 0;
	}

private:
	enum LogType
	{
		Message,	// white text
		Error,		// red text
		Info,		// yellow text
	};

	struct LogEntry
	{
		LogType type;
		String message;
	};

	StaticArray<char, 255> inputBuf;
	Array<LogEntry> log;
	Array<String> history;
	int historyIndex;
	bool autoScoll;
	StringBuilder strBuilder;
};