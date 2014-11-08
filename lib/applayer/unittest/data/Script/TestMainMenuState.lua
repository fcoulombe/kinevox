function Initialize(self)
  kinevox.Log("Main Menu!")
  frameCount = 0
end

function Logic(self, dt)
  if frameCount == 0 then
    kinevox.PushState("TestConfigState", "TestConfigState")
  elseif frameCount == 1 then
    kinevox.ChangeToState("TestMainGameState", "TestMainGameState")
  end
  frameCount = frameCount + 1
end

function Terminate(self)

end