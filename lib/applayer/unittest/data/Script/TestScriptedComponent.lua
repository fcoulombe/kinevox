
function Initialize()
  SCREEN_SIZE = kinevox.GetScreenSize()
  kinevox.Log("TestInit"..SCREEN_SIZE[0].." "..SCREEN_SIZE[1].." "..KINEVOS_ACTOR_ID)
  assert(SCREEN_SIZE[0]==640)
  assert(SCREEN_SIZE[1]==480)
  kinevox.Log("TestInit")
  s = 1
  pos = kinevox.GetPosition()
  kinevox.Log("position: "..pos[0].." "..pos[1].." "..pos[2])
  
end

function Logic()
  s = s+s
  kinevox.Log("TestLog"..s)
  
  pos = kinevox.GetPosition()
  kinevox.Log("position: "..pos[0].." "..pos[1].." "..pos[2])
end


function Terminate()
  kinevox.Log("TestTerminate")
end