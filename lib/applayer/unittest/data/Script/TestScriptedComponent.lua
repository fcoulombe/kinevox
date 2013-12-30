
function Initialize()
  SCREEN_SIZE = kinevox.GetScreenSize()
  kinevox.Log("TestInit"..SCREEN_SIZE[0].." "..SCREEN_SIZE[1])
  assert(SCREEN_SIZE[0]==640)
  assert(SCREEN_SIZE[1]==480)
  kinevox.Log("TestInit")
  s = 1
end

function Logic()
  s = s+s
  kinevox.Log("TestLog"..s)
end


function Terminate()
  kinevox.Log("TestTerminate")
end