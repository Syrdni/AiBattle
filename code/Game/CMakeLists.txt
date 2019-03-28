fips_begin_app(Game windowed)
	fips_files(
		DataContainerSingleton.h
		Game.h Game.cpp
		PyConsole.h
	)
	fips_libs(cjson)
	fips_deps(Gfx LocalFS Input Assets IMUI Resource ECS)
	oryol_shader(Shaders/shaders.shd)
	fips_dir(Components)
	fips_files(
		TransformComponent.h TransformComponent.cpp
		ShapeRenderer.h ShapeRenderer.cpp
		# units.
		Explorer.h
		Worker.h
		Craftsman.h
		Soldier.h
		# good to have.
		TeamTag.h
		PathFollower.h PathFollower.cpp
		HealthComponent.h HealthComponent.cpp
		LevelVisitor.h
		Discoverable.h
		# special components for time taking.
		MineChop.h
		CreateItem.h
		BuildBuilding.h
		Upgrading.h
		# time taking components.
		Progress.h
		InventoryComponent.h InventoryComponent.cpp
		# buildings
		KilnComponent.h
		BuildingSiteComponent.h
		SmelterComponent.h
		TrainingCampComponent.h
		SmithyComponent.h
		TaskPlanner.h
	)
	fips_dir(World)
	fips_files(
		Level.h Level.cpp 
		PathPlanner.cpp PathPlanner.h
		ShapeServer.h ShapeServer.cpp
		MessageDispatcher.h MessageDispatcher.cpp
		Tile.h Tile.cpp
		AICore.h
	)
	fips_dir(Messages)
	fips_files(
		TileChange.h
		EntityRemove.h
		Deposit.h
		PathArrive.h
		PathCancel.h
		Attack.h
		Heal.h
		ProgressMsg.h
		UpgradeComplete.h
		AttackCooldown.h
		Abort.h
	)
fips_end_app()

# copy model files to build tree
FILE(GLOB_RECURSE ASSETS Assets/*)
foreach(ASSET ${ASSETS})
	configure_file(${ASSET} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endforeach(ASSET)

# link python37.lib
target_link_libraries(Game ${PYTHON_LIBRARY_RELEASE})