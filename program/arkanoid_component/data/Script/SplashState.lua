function Initialize(self)
  SCREEN_SIZE = kinevox.GetScreenSize()
  kinevox.CreateActor("Splash", "Splash")
  splashSprite = kinevox.GetActor("Splash")
  splashSprite:SetPosition(SCREEN_SIZE[0]/2, SCREEN_SIZE[1]/2, 0)
  timeUntilNextState = 0.0
  sound.PlayMusic("intro_loop.ogg") 
end

function Logic(self, dt)
  timeUntilNextState = timeUntilNextState + dt
  if timeUntilNextState > 3.0 then
    kinevox.ChangeToState("MainMenuState", "MainMenuState")
  end
end


function Terminate(self)
  kinevox.DestroyActor("Splash")
end
