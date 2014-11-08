function Initialize(self)
  kinevox.Log("Main Game State!")
  SCREEN_SIZE = kinevox.GetScreenSize()
    
  kinevox.CreateActor("Road1", "Road")
  local road = kinevox.GetActor("Road1")
  road:SetPosition(SCREEN_SIZE[0]/2, SCREEN_SIZE[1]/2, 0)
  
  kinevox.CreateActor("Road2", "Road")
  local road = kinevox.GetActor("Road2")
  road:SetPosition(SCREEN_SIZE[0]/2, SCREEN_SIZE[1]/2 - SCREEN_SIZE[1], 0)
  
  kinevox.CreateActor("Car", "Car")
  local car = kinevox.GetActor("Car")
  car:SetPosition(SCREEN_SIZE[0]/2, SCREEN_SIZE[1]/2 + 100, 1)
  
  for i=1,9 do
    local mineName = "LMine"..i
    kinevox.CreateActor(mineName, "Mine")
    local mine = kinevox.GetActor(mineName)
    mine:SetPosition(SCREEN_SIZE[0]/2-120,  (i-1)*64, 1)
  end
  for i=1,9 do
    local mineName = "RMine"..i
    kinevox.CreateActor(mineName, "Mine")
    local mine = kinevox.GetActor(mineName)
    mine:SetPosition(SCREEN_SIZE[0]/2+120,  (i-1)*64, 1)
  end
end

function Logic(self, dt)
  local road1 = kinevox.GetActor("Road1")
  local roadPos = road1:GetPosition()
  roadPos[1] = roadPos[1] + 1.0
  if roadPos[1] > SCREEN_SIZE[1]/2 + SCREEN_SIZE[1] then
    roadPos[0] = SCREEN_SIZE[0]/2
    roadPos[1] = SCREEN_SIZE[1]/2 - SCREEN_SIZE[1]
    roadPos[2] = 0
  end
  road1:SetPosition(roadPos[0], roadPos[1], roadPos[2])
  
  
  local road2 = kinevox.GetActor("Road2")
  local roadPos = road2:GetPosition()
  roadPos[1] = roadPos[1] + 1.0
  if roadPos[1] > SCREEN_SIZE[1]/2 + SCREEN_SIZE[1] then
    roadPos[0] = SCREEN_SIZE[0]/2
    roadPos[1] = SCREEN_SIZE[1]/2 - SCREEN_SIZE[1]
    roadPos[2] = 0
  end
  road2:SetPosition(roadPos[0], roadPos[1], roadPos[2])
  
end

function Terminate(self)
  kinevox.DestroyActor("Car")
  kinevox.DestroyActor("Road2")
  kinevox.DestroyActor("Road1")
  for i=1,9 do
    local mineName = "RMine"..i
    kinevox.DestroyActor(mineName)
    local mineName = "LMine"..i
    kinevox.DestroyActor(mineName)
  end
end