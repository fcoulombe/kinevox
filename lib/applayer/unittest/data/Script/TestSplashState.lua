function Initialize(self)
  kinevox.Log("splash!")
end

function Logic(self, dt)
  kinevox.ChangeToState("TestMainMenuState", "TestMainMenuState")
end

function Terminate(self)

end