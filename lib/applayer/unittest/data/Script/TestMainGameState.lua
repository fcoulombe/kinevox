function Initialize(self)
  kinevox.Log("Main Game State!")
  kinevox.CreateActor("TestActor", "TestActor")
  frameCount = 0
  actor = kinevox.GetActor("TestActor")
  actor:SetPosition(0.0,0.0,-10.0)
end

function Logic(self, dt)
  if frameCount>3 then
    kinevox.PushChildState("TestPauseState", "TestPauseState")
  end
  frameCount = frameCount + 1
end

function Terminate(self)
  kinevox.DestroyActor("TestActor")
end