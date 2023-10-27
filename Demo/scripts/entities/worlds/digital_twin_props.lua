function init()
    scene.globals.benches = {};
    scene.globals.chairs = {};
    scene.globals.tables = {};
    scene.globals.rubbishBins = {};
    scene.globals.recyclingBins = {};
	scene.globals.bins = {};
    scene.globals.apples = {};
    scene.globals.drinkCups = {};
    scene.globals.beerCans = {};

	local soccerBall;

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
		Vector3.new(33.0, 1.221, 39.0), Vector3.new(-16.3, 1.321, 84.0)
	}

	local recyclingBinPositions = 
	{
		Vector3.new(31.4, 1.221, 39.0), Vector3.new(-16.3, 1.221, 85.6)
	}

	local applePositions = 
	{
		Vector3.new(18.8, 1.0, 22.9), Vector3.new(-11.3, 1.0, -32.9), Vector3.new(15.5, 1.0, -12.8),
		Vector3.new(-21.4, 1.0, 42.4), Vector3.new(-24.3, 1.0, 98.4), Vector3.new(2.0, 1.0, 3.0)
	}

	local drinkCupPositions = 
	{
		Vector3.new(12.5, 1.0, 4.0), Vector3.new(0.9, 1.0, 28.9), Vector3.new(4.4, 1.0, -13.3),
		Vector3.new(-26.0, 1.0, 56.7)
	}

	local beerCanPositions = 
	{
		Vector3.new(9.2, 1.0, 12.2), Vector3.new(-13.0, 1.0, 2.0)
	}

	--ENTITY CREATION & SETUP
	--Digital Twin Prop Entities
	for i = 1, 6 do
		local entityName = "Bench";
		entityName = entityName .. " " .. i;
	
		scene.globals.benches[i] = scene:create_entity(entityName, "entities/objects/bench.lua");

		scene.globals.benches[i]:get_transform().pos = benchPositions[i];
	end

	for i = 1, 20 do
		local entityName = "Chair";
		entityName = entityName .. " " .. i;
	
		scene.globals.chairs[i] = scene:create_entity(entityName, "entities/objects/chair.lua");

		scene.globals.chairs[i]:get_transform().pos = chairPositions[i];
		scene.globals.chairs[i]:get_transform().rot = chairRotations[i];

	end

	for i = 1, 4 do
		local entityName = "Table";
		entityName = entityName .. " " .. i;
	
		scene.globals.tables[i] = scene:create_entity(entityName, "entities/objects/table.lua");

		scene.globals.tables[i]:get_transform().pos = tablePositions[i];
	end

    for i = 1, 2 do
        local rubbishBinName = "Rubbish Bin " .. i;

        scene.globals.rubbishBins[i] = scene:create_entity(rubbishBinName, "entities/objects/rubbish_bin.lua");

        local recyclyingBinName = "Recycling Bin " .. i;

        scene.globals.recyclingBins[i] = scene:create_entity(recyclyingBinName, "entities/objects/recycle_bin.lua");

        scene.globals.rubbishBins[i]:get_transform().pos = rubbishBinPositions[i];
        scene.globals.recyclingBins[i]:get_transform().pos = recyclingBinPositions[i];

        if i == 2 then
            scene.globals.rubbishBins[i]:get_transform().rot = Vector3.new(0.0, 90.0, 0.0);
            scene.globals.recyclingBins[i]:get_transform().rot = Vector3.new(0.0, 90.0, 0.0);
        end

        -- Add bins to the set of all bins
        scene.globals.bins[rubbishBinName] = scene.globals.rubbishBins[i];
        scene.globals.bins[recyclyingBinName] = scene.globals.recyclingBins[i];
    end
	

	for i = 1, 6 do
		local entityName = "Apple";
		entityName = entityName .. " " .. i;
	
		scene.globals.apples[i] = scene:create_entity(entityName, "entities/objects/apple.lua");

		scene.globals.apples[i]:get_transform().pos = applePositions[i];
	end

	for i = 1, 4 do
		local entityName = "Drink Cup";
		entityName = entityName .. " " .. i;
	
		scene.globals.drinkCups[i] = scene:create_entity(entityName, "entities/objects/drink_cup.lua");

		scene.globals.drinkCups[i]:get_transform().pos = drinkCupPositions[i];
	end

	for i = 1,2 do
		local entityName = "Beer Can";
		entityName = entityName .. " " .. i;
	
		scene.globals.beerCans[i] = scene:create_entity(entityName, "entities/objects/beer_can.lua");

		scene.globals.beerCans[i]:get_transform().pos = beerCanPositions[i];
	end
	
	soccerBall = scene:create_entity("Soccer Ball", "entities/objects/soccer_ball.lua");

	soccerBall:get_transform().pos = Vector3.new(0.0, 2.0, 0.0);
end