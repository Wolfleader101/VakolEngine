
ANGER <const> = 0;
ANTICIPATION <const> = 1;
JOY <const> = 2;
TRUST <const> = 3;
FEAR <const> = 4;
SURPRISE <const> = 5;
SADNESS <const> = 6;
DISGUST <const> = 7;


local emotion_concepts = { 0  ,  0  , 0  , 0  , 0  , 0  , 0  , 0   };

local decays           = { 0.05, 0.03, 0.1, 0.001, 0.01, 0.15, 0.025, 0.01 };

--whipped these values out me dot
local weights = {
    {  0   ,  0.2  , -0.525, -0.3  , -0.8  ,  0.1  ,  0.2  ,  0.3  },
    { -0.1 ,  0    ,  0.1  ,  0    ,  0.5  , -0.8  ,  0    , -0.2  },
    { -0.3 ,  0.05 ,  0    ,  0.175, -0.3  ,  0.2  , -0.8  , -0.5  },
    { -0.6 ,  0    ,  0.4  ,  0    , -0.6  , -0.1  , -0.3  , -0.8  },
    { -0.8 ,  0.05 ,  0    , -0.2  ,  0    ,  0    ,  0.15 ,  0.3  },
    {  0   , -0.8  ,  0.25 ,  0    ,  0.3  ,  0    , -0.3  ,  0    },
    {  0.5 ,  0    , -0.8  , -0.1  ,  0.2  ,  0    ,  0    ,  0.15 },
    {  0.1 ,  0.2  , -0.45 , -0.8  ,  0.1  ,  0    ,  0.1  ,  0    }
}


function get_emotion(emotion_index)
    
    if emotion_index < 0 or emotion_index > 7 then
        return -1
    end

    return emotion_concepts[emotion_index]

end

function set_emotion(emotion_index, value)

    if emotion_index < 0 or emotion_index > 7 then
        return
    end

    emotion_concepts[emotion_index] = value

end

local function iterate()

end


function init()

end


function update()

end


function tick()

end

function phys_update()

end
