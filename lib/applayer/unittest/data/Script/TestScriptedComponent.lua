
function Initialize()
  kinevox.Log("TestInit")
  s = 1
end

function TestKinevoxModule()
  --Test Log
  s = s+s
  kinevox.Log("TestLog"..s)
  
    --Test GetScreen size
  SCREEN_SIZE = kinevox.GetScreenSize()
  kinevox.Log("TestInit"..SCREEN_SIZE[0].." "..SCREEN_SIZE[1].." "..KINEVOS_ACTOR_ID)
  assert(SCREEN_SIZE[0]==640)
  assert(SCREEN_SIZE[1]==480)
  
  --test GetActor and Get/SetPositoin
  scriptedActor2 = kinevox.GetActor("TestScriptedActor2")
  kinevox.SetPosition(scriptedActor2, 444, 555, 666)
  scriptedActorPos = kinevox.GetPosition(scriptedActor2)
  
  kinevox.Log("ScriptedAcotr position: "..scriptedActorPos[0].." "..scriptedActorPos[1].." "..scriptedActorPos[2])
  assert(scriptedActorPos[0] == 444 and scriptedActorPos[1] == 555 and scriptedActorPos[2] == 666)
end

function TestObjectModule()
  --Test Get/Set Object
  object.SetPosition(10,20)
  pos = object.GetPosition()
  assert(pos[0] == 10 and pos[1] == 20)
  kinevox.Log("position: "..pos[0].." "..pos[1].." "..pos[2])
end

function TestInputModule()
  --Test IsKeyDown
  local isKeyDown = false
  if input.IsKeyDown(GCL_RIGHT) then
    isKeyDown = true
  end
end

function Logic()
  TestKinevoxModule()
  TestObjectModule()
  TestInputModule()
end


function Terminate()
  kinevox.Log("TestTerminate")
end