
ANGER = 1;
ANTICIPATION = 2;
JOY = 3;
TRUST = 4;
FEAR = 5;
SURPRISE = 6;
SADNESS = 7;
DISGUST = 8;


local emotion_names = {"Anger", "Anticipation", "Joy", "Trust", "Fear", "Surprise", "Sadness", "Disgust"};
local emotion_concepts = { -0.4  ,     0  ,    0.96  ,   0.5  ,  -0.9  ,    0  ,      0.1  ,     -0.9   };

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

local function tanh(x) --activation function. bounds between -1 and 1
    local e_pos = math.exp(x);  
    local e_neg = math.exp(-x);
    return (e_pos - e_neg) / (e_pos + e_neg);
end

local function iterate()

    new_concepts = {};

    for i = 1, 8 do
        sum = 0;

        for j = 1, 8 do
            sum = sum + emotion_concepts[j] * weights[j][i] -- accidentally made transpose so reverse indexing gg lmao
        end
        
        new_concepts[i] = emotion_concepts[i] + sum - decays[i];
    end

    for i = 1, 8 do
        emotion_concepts[i] = new_concepts[i]
    end
end



-- ================== VAKOL FUNCS

function init()

end


function update()

end


function tick()
    iterate();
    print_emotions();
end

function phys_update()

end
