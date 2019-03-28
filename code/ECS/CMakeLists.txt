fips_begin_module(ECS)
	fips_vs_warning_level(3)
	fips_files(
		Entity.h Entity.cpp
		EntityManager.h EntityManager.cpp 
		ECSUtil.h 
		BaseComponent.h
		Message.h
		Attribute.h
		IdentifierPool.h
	)
fips_end_module()