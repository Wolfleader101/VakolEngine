
ANGER = 1;
ANTICIPATION = 2;
JOY = 3;
TRUST = 4;
FEAR = 5;
SURPRISE = 6;
SADNESS = 7;
DISGUST = 8;

local tickSkips <const> = 3;
local emotion_names = {"Anger", "Anticipation", "Joy", "Trust", "Fear", "Surprise", "Sadness", "Disgust"};
local emotion_concepts = { 0.2  ,     0.1   ,    0.6  ,   0.3  ,  -0.4  ,    0.1  ,      0.1  ,     0.1   };

local decays           = { 0.01  ,     0.03   ,    0.03  ,   0.03  ,  0.03  ,    0.2   ,     0.01  ,     0.001 };

--whipped these values out me dot
-- local weights = {
--     {  0.1   ,  0.02  , -0.05  , -0.03  , -0.08  ,  0.01  ,  0.02  ,  0.03  },
--     { -0.01  ,  -0.1  ,  0.01  ,  0     ,  0.05  , -0.08  ,  0     , -0.02  },
--     { -0.05  ,  0.005 ,  -0.1  ,  0.0175, -0.03  ,  0.02  , -0.08  , -0.05  },
--     { -0.06  ,  0     ,  0.04  ,  -0.1  , -0.06  , -0.01  , -0.03  , -0.08  },
--     { -0.08  ,  0.005 ,  0     , -0.02  ,  -0.1  ,  0     ,  0.015 ,  0.03  },
--     {  0     , -0.08  ,  -0.025 ,  0     ,  -0.03  ,  -0.1  , -0.03  ,  0   },
--     {  0.05  ,  0     , -0.08  , -0.01  ,  0.02  ,  0     ,  -0.1  ,  0.015 },
--     {  0.01  ,  0.02  , -0.045 , -0.08  ,  0.01  ,  0     ,  0.01  ,  -0.1  }
-- }

local weights = { --symmetrical matrix
    {  0.1   ,  0.02  , -0.05  , -0.03  , -0.08  ,  0.01  ,  0.02  ,  0.03  },
    {  0.02  , 0.1    ,  0.01  ,  0     ,  0.05  , -0.08  ,  0     , -0.02  },
    { -0.05  ,  0.01  ,    0   ,  0.0175, -0.03  ,  0.02  , -0.08  , -0.05  },
    { -0.03  ,  0     ,  0.0175,    0   , -0.06  , -0.01  , -0.03  , -0.08  },
    { -0.08  ,  0.05  , -0.03  , -0.06  , -0.1   ,  0     ,  0.015 ,  0.03  },
    {  0.01  , -0.08  ,  0.02  , -0.01  ,  0     , -0.2   , -0.03  ,  0     },
    {  0.02  ,  0     , -0.08  , -0.03  ,  0.015 , -0.03  , -0.1   ,  0.015 },
    {  0.03  , -0.02  , -0.05  , -0.08  ,  0.03  ,  0     ,  0.015 , -0.1   }
}

-- local weights = {
--     {   0   ,  0.02  , -0.05  , -0.03  , -0.08  ,  0.01  ,  0.02  ,  0.03  },
--     {  0.02  ,  0  ,  0.01  ,  0     ,  0.05  , -0.08  ,  0     , -0.02  },
--     { -0.05  ,  0.01  ,  0,  0.0175, -0.03  ,  0.02  , -0.08  , -0.05  },
--     { -0.03  ,  0     ,  0.0175,  0 , -0.06  , -0.01  , -0.03  , -0.08  },
--     { -0.08  ,  0.05  , -0.03  , -0.06  ,  0  ,  0     ,  0.015 ,  0.03  },
--     {  0.01  , -0.08  ,  0.02  , -0.01  ,  0     ,  0  , -0.03  ,  0     },
--     {  0.02  ,  0     , -0.08  , -0.03  ,  0.015 , -0.03  ,  0 ,  0.015 },
--     {  0.03  , -0.02  , -0.05  , -0.08  ,  0.03  ,  0     ,  0.015 ,  0 }
-- }

function print_emotions()
    local output = ""
    for i = 1, 8 do
        output = output .. emotion_names[i] .. ": " .. emotion_concepts[i] .. " | "
    end
    print(output)
end



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

function tanh(x)
    local epx = math.exp(x)
    local enx = math.exp(-x)
    return (epx - enx) / (epx + enx)
end

local function normalize()
    local max_val = emotion_concepts[1]
    local min_val = max_val

    for i = 1, 8 do
        if emotion_concepts[i] > max_val then
            max_val = emotion_concepts[i]
        elseif emotion_concepts[i] < min_val then
            min_val = emotion_concepts[i]
        end
    end

    local range = max_val - min_val
    
    for i = 1, 8 do
        emotion_concepts[i] = 2 * ((emotion_concepts[i] - min_val) / range) - 1
    end
end

local function L2Normalize()
    
    L2Norm = 0;

    for i = 1, #emotion_concepts do
        L2Norm = L2Norm + emotion_concepts[i] * emotion_concepts[i]; --^2
    end

    L2Norm = math.sqrt(L2Norm);

    for i = 1, #emotion_concepts do
         emotion_concepts[i] = (emotion_concepts[i] / L2Norm);
    end
end

local function iterate()

    new_concepts = {};

    for i = 1, 8 do
        sum = 0;

        for j = 1, 8 do
            sum = sum + emotion_concepts[j] * weights[j][i] -- accidentally made transpose so reverse indexing gg lmao
        end
        
        new_concepts[i] = (emotion_concepts[i] + sum) * ( 1 - decays[i] );
    end

    for i = 1, 8 do
        emotion_concepts[i] = new_concepts[i];
    end

    L2Normalize();
end



-- ================== VAKOL FUNCS

function init()

end


function update()

    if (Input:get_key_down(KEYS["KEY_Z"])) then
        set_emotion(ANGER, 0.5);
    end

    if (Input:get_key_down(KEYS["KEY_X"])) then
        set_emotion(ANTICIPATION, 5);
    end

    if (Input:get_key_down(KEYS["KEY_C"])) then
        set_emotion(JOY, 5);
    end

    if (Input:get_key_down(KEYS["KEY_V"])) then
        set_emotion(TRUST, 5);
    end

    if (Input:get_key_down(KEYS["KEY_B"])) then
        set_emotion(FEAR, 5);
    end

    if (Input:get_key_down(KEYS["KEY_N"])) then
        set_emotion(SURPRISE, 5);
    end

    if (Input:get_key_down(KEYS["KEY_M"])) then
        set_emotion(SADNESS, 5);
    end

    if (Input:get_key_down(KEYS["KEY_L"])) then
        set_emotion(DISGUST, 10);
    end
end

local slow = 0;

function tick()

    if (slow == 5) then
        iterate();
        print_emotions();
        slow = 0;
    else
        slow = slow + 1;

    end
end

function phys_update()

end
