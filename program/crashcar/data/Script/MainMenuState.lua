function Initialize(self)
  kinevox.Log("Main Menu!")
  SCREEN_SIZE = kinevox.GetScreenSize()
  kinevox.CreateActor("MainMenu", "MainMenu")
  splashSprite = kinevox.GetActor("MainMenu")
  splashSprite:SetPosition(SCREEN_SIZE[0]/2, SCREEN_SIZE[1]/2, 0)
end

function Logic(self, dt)
  
  if input.IsKeyDown(GCL_ENTER) then
    kinevox.Log("Logic!!")
    kinevox.ChangeToState("MainGameState", "MainGameState")
  end
end

function Terminate(self)
  kinevox.DestroyActor("MainMenu")
end