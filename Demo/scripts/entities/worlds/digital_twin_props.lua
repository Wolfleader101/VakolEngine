function init()
	local benches = {};
	local chairs = {};
	local tables = {};
	local rubbishBins = {};
	local recyclingBins = {};

	GLOBAL_SCALE = 0.02;

	--OBJECT POSITIONS
	local benchPositions = 
	{
		Vector3.new(-27.6, 1.05, 114.54), Vector3.new(-21.1, 1.05, 114.58),
		Vector3.new(-21.5, 1.05, 123.6), Vector3.new(-27.9, 1.05, 123.7), Vector3.new(-21.5, 1.05, 132.58), 
		Vector3.new(-28.6, 1.05, 131.8)
	};

	local tablePositions = 
	{
		Vector3.new(-17.9, 1.051, 44.2), Vector3.new(-21.2, 1.051, 48.7), Vector3.new(-21.3, 1.051, 54.1),
		Vector3.new(-20.9, 1.051, 60.8)
	};

	local chairRotations = 
	{
		Vector3.new(0.0, -102.0, 0.0), Vector3.new(0.0, 87.0, 0.0), Vector3.new(0.0, 92.6, 0.0),
		Vector3.new(0.0, 254.2, 0.0), Vector3.new(0.0, 280.4, 0.0), Vector3.new(0.0, 271.4, 0.0),
		Vector3.new(0.0, 433.4, 0.0), Vector3.new(0.0, 89.0, 0.0), Vector3.new(0.0, 94.0, 0.0),
		Vector3.new(0.0, 90.6, 0.0), Vector3.new(0.0, 270.7, 0.0), Vector3.new(0.0, 262.8, 0.0),
		Vector3.new(0.0, 273.5, 0.0), Vector3.new(0.0, -82.1, 0.0), Vector3.new(0.0, 73.1, 0.0),
		Vector3.new(0.0, 92.2, 0.0), Vector3.new(0.0, 175.6, 0.0), Vector3.new(0.0, 182.1, 0.0),
		Vector3.new(0.0, 180.2, 0.0), Vector3.new(0.0, 174.3, 0.0)
	};

	local chairPositions = 
	{
		Vector3.new(-17.3, 1.16, 45.6), Vector3.new(-17.3, 1.16, 42.6), Vector3.new(-18.6, 1.16, 43.0),
		Vector3.new(-19.0, 1.16, 46.3), Vector3.new(-20.8, 1.16, 49.7), Vector3.new(-22.1, 1.16, 50.0),
		Vector3.new(-21.9, 1.16, 47.5), Vector3.new(-20.6, 1.16, 47.9), Vector3.new(-20.4, 1.16, 53.3),
		Vector3.new(-22.0, 1.16, 53.0), Vector3.new(-20.8, 1.16, 55.2), Vector3.new(-22.1, 1.16, 55.4),
		Vector3.new(-20.2, 1.16, 61.7), Vector3.new(-21.6, 1.16, 61.9), Vector3.new(-20.1, 1.16, 59.8),
		Vector3.new(-21.7, 1.16, 59.6), Vector3.new(-17.2, 1.16, 56.5), Vector3.new(-17.5, 1.16, 58.5),
		Vector3.new(-17.3, 1.16, 60.2), Vector3.new(-17.4, 1.16, 61.9)
	}

	local rubbishBinPositions = 
	{
		Vector3.new(33.0, 1.221, 39.0), Vector3.new(-16.3, 1.221, 84.0)
	}

	local recyclingBinPositions = 
	{
		Vector3.new(31.4, 1.221, 39.0), Vector3.new(-16.3, 1.221, 85.6)
	}

	--ENTITY CREATION & SETUP
	--Digital Twin Prop Entities
	for i = 1, 6 do
		local entityName = "Bench";
		entityName = entityName .. " " .. i;
	
		benches[i] = scene:create_entity(entityName, "");

		benches[i]:add_model("assets/models/imported/OpenGameArt/Teh_Bucket/Bench/bench.fbx", GLOBAL_SCALE);

		benches[i]:get_transform().pos = benchPositions[i];

		local benchesRigid = benches[i]:add_rigid();

		benchesRigid.type = BodyType.Static;

		benches[i]:add_box_collider(Vector3.new(2.64, 1.016, 2.158));
	end

	for i = 1, 20 do
		local entityName = "Chair";
		entityName = entityName .. " " .. i;
	
		chairs[i] = scene:create_entity(entityName, "");

		chairs[i]:add_model("assets/models/imported/OpenGameArt/loafbrr_1/Furniture/Furniture_Chair_1.fbx", GLOBAL_SCALE);

		chairs[i]:get_transform().pos = chairPositions[i];
		chairs[i]:get_transform().rot = chairRotations[i];

		--local chairsRigid = chairs[i]:add_rigid();

		--chairsRigid.type = BodyType.Static;

		--chairs[i]:add_box_collider(Vector3.new(0.7, 1.15, 0.5));
	end

	for i = 1, 4 do
		local entityName = "Table";
		entityName = entityName .. " " .. i;
	
		tables[i] = scene:create_entity(entityName, "");

		tables[i]:add_model("assets/models/imported/OpenGameArt/loafbrr_1/Furniture/Furniture_Table_2.fbx", GLOBAL_SCALE);

		tables[i]:get_transform().pos = tablePositions[i];

		local tablesRigid = tables[i]:add_rigid();

		tablesRigid.type = BodyType.Static;
	end

	tables[1]:add_box_collider(Vector3.new(1.9, 1.03, 2.0));
	tables[2]:add_box_collider(Vector3.new(1.9, 1.03, 1.7));
	tables[3]:add_box_collider(Vector3.new(1.8, 1.03, 1.5));
	tables[4]:add_box_collider(Vector3.new(1.9, 1.03, 1.6));

	for i = 1, 2 do
		local entityName = "Rubbish Bin";
		entityName = entityName .. " " .. i;
	
		rubbishBins[i] = scene:create_entity(entityName, "");

		entityName = "Recycling Bin";
		entityName = entityName .. " " .. i;

		recyclingBins[i] = scene:create_entity(entityName, "");

		rubbishBins[i]:add_model("assets/models/imported/OpenGameArt/PagDev/Trashcan/rubbishBin_Rubbish.fbx", GLOBAL_SCALE);
		recyclingBins[i]:add_model("assets/models/imported/OpenGameArt/PagDev/Trashcan/rubbishBin_Recycling.fbx", GLOBAL_SCALE);

		rubbishBins[i]:get_transform().pos = rubbishBinPositions[i];
		recyclingBins[i]:get_transform().pos = recyclingBinPositions[i];

		if i == 2 then
			rubbishBins[i]:get_transform().rot = Vector3.new(0.0, 90.0, 0.0);
			recyclingBins[i]:get_transform().rot = Vector3.new(0.0, 90.0, 0.0);
		end

		local rubbishRigid = rubbishBins[i]:add_rigid();
		local recyclingRigid = recyclingBins[i]:add_rigid();

		rubbishRigid.type = BodyType.Static;
		recyclingRigid.type = BodyType.Static;

		rubbishBins[i]:add_box_collider(Vector3.new(0.6, 1.2, 0.9));
		recyclingBins[i]:add_box_collider(Vector3.new(0.6, 1.2, 0.9));
	end
end