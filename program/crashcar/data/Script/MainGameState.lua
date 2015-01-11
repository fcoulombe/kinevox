MINE_COUNT = 9
VELOCITY = { -0.0,50.0} 
function Initialize(self)
  kinevox.Log("Main Game State!")
  SCREEN_SIZE = kinevox.GetScreenSize()
  HALF_SCREEN_WIDTH = SCREEN_SIZE[0]/2
  HALF_SCREEN_HEIGHT = SCREEN_SIZE[1]/2
    
  kinevox.CreateActor("Road1", "Road")
  local road = kinevox.GetActor("Road1")
  road:SetPosition(HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT, 0.0)
  
  kinevox.CreateActor("Road2", "Road")
  local road = kinevox.GetActor("Road2")
  road:SetPosition(HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT - SCREEN_SIZE[1]+1, 0.0)
  
  kinevox.CreateActor("Car", "Car")
  local car = kinevox.GetActor("Car")
  car:SetPosition(HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT + 100, 1)
  
  for i=1,MINE_COUNT do
    local mineName = "LMine"..i
    kinevox.CreateActor(mineName, "Mine")
    local mine = kinevox.GetActor(mineName)
    mine:SetPosition(HALF_SCREEN_WIDTH-120,  (i-1)*64-32, 0.5)
  end
  for i=1,MINE_COUNT do
    local mineName = "RMine"..i
    kinevox.CreateActor(mineName, "Mine")
    local mine = kinevox.GetActor(mineName)
    mine:SetPosition(HALF_SCREEN_WIDTH+120,  (i-1)*64-32, 0.5)
  end
end

function Logic(self, dt)
  --kinevox.Log("dt: "..dt.." Velocity: "..VELOCITY[1].." "..VELOCITY[2]) 
  local road1 = kinevox.GetActor("Road1")
  local roadPos = road1:GetPosition()
  roadPos[1] = roadPos[1] + VELOCITY[2] * dt
  if roadPos[1] > HALF_SCREEN_HEIGHT + SCREEN_SIZE[1]-1 then
    roadPos[0] = HALF_SCREEN_WIDTH
    roadPos[1] = HALF_SCREEN_HEIGHT - SCREEN_SIZE[1]
    roadPos[2] = roadPos[2]
    kinevox.Log("road1 pos: "..roadPos[0].." "..roadPos[1])
  end
  road1:SetPosition(roadPos[0], roadPos[1], roadPos[2])
  
  
  local road2 = kinevox.GetActor("Road2")
  local roadPos = road2:GetPosition()
  roadPos[1] = roadPos[1] + VELOCITY[2] * dt
  if roadPos[1] > HALF_SCREEN_HEIGHT + SCREEN_SIZE[1]-1 then
    roadPos[0] = HALF_SCREEN_WIDTH
    roadPos[1] = HALF_SCREEN_HEIGHT - SCREEN_SIZE[1]
    roadPos[2] = roadPos[2]
    
    kinevox.Log("road2 pos: "..roadPos[0].." "..roadPos[1])
  end
  road2:SetPosition(roadPos[0], roadPos[1], roadPos[2])
  
end

function Terminate(self)
  kinevox.DestroyActor("Car")
  kinevox.DestroyActor("Road2")
  kinevox.DestroyActor("Road1")
  for i=1,MINE_COUNT do
    local mineName = "RMine"..i
    kinevox.DestroyActor(mineName)
    local mineName = "LMine"..i
    kinevox.DestroyActor(mineName)
  end
end