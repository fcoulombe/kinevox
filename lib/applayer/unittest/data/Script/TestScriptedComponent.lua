
function Initialize()
  SCREEN_SIZE = kinevox.GetScreenSize()
  kinevox.Log("TestInit"..SCREEN_SIZE[0].." "..SCREEN_SIZE[1].." "..KINEVOS_ACTOR_ID)
  assert(SCREEN_SIZE[0]==640)
  assert(SCREEN_SIZE[1]==480)
  kinevox.Log("TestInit")
  s = 1
  pos = object.GetPosition()
  kinevox.Log("position: "..pos[0].." "..pos[1].." "..pos[2])
  
end

function Logic()
  s = s+s
  kinevox.Log("TestLog"..s)
  
  object.SetPosition(10,20)
  pos = object.GetPosition()
  assert(pos[0] == 10 and pos[1] == 20)
  kinevox.Log("position: "..pos[0].." "..pos[1].." "..pos[2])
  local isKeyDown = false
  if input.IsKeyDown(GCL_RIGHT) then
    isKeyDown = true
  end
end


function Terminate()
  kinevox.Log("TestTerminate")
end