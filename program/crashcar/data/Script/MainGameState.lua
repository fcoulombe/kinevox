MINE_COUNT = 17
VELOCITY = { -0.0,150.0}
START_WIDTH = 200
END_WIDTH = 50
TOP_MINE_POS = {0.0, 0.0, 0.5}

function CreateRoad(self)
  kinevox.CreateActor("Road1", "Road")
  local road = kinevox.GetActor("Road1")
  road:SetPosition(HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT, 0.0)
  road:SetVisible(true)
  kinevox.CreateActor("Road2", "Road")
  local road = kinevox.GetActor("Road2")
  road:SetPosition(HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT - SCREEN_SIZE[1], 0.0)
  road:SetVisible(true)
end

function CreateCar(self)
  kinevox.CreateActor("Car", "Car")
  local car = kinevox.GetActor("Car")
  car:SetPosition(HALF_SCREEN_WIDTH, HALF_SCREEN_HEIGHT + 100, 1)
end

function CreateMines()
  TOP_MINE_POS[1] = HALF_SCREEN_WIDTH
  for i=1,MINE_COUNT do
    local mineName = "LMine"..i
    kinevox.CreateActor(mineName, "Mine")
    local mine = kinevox.GetActor(mineName)
    mine:SetPosition(TOP_MINE_POS[1]-START_WIDTH,  (i-1)*32-16, 0.5)
  end
  for i=1,MINE_COUNT do
    local mineName = "RMine"..i
    kinevox.CreateActor(mineName, "Mine")
    local mine = kinevox.GetActor(mineName)
    mine:SetPosition(TOP_MINE_POS[1]+START_WIDTH,  (i-1)*32-16, 0.5)
  end
end

function Initialize(self)
  kinevox.Log("Main Game State!")
  SCREEN_SIZE = kinevox.GetScreenSize()
  HALF_SCREEN_WIDTH = math.floor(SCREEN_SIZE[0]/2 +0.5)
  HALF_SCREEN_HEIGHT = math.floor(SCREEN_SIZE[1]/2 +0.5)
  self:CreateRoad()
  self:CreateCar()
  self:CreateMines()
  
end

function RoadLogic(dt)
  local velocityY = VELOCITY[2] * dt
  
  local road1 = kinevox.GetActor("Road1")
  local roadPos = road1:GetPosition()
  roadPos[1] = roadPos[1] + velocityY
  if roadPos[1] > HALF_SCREEN_HEIGHT + SCREEN_SIZE[1] then
    roadPos[0] = HALF_SCREEN_WIDTH
    roadPos[1] = HALF_SCREEN_HEIGHT - SCREEN_SIZE[1]
    roadPos[2] = roadPos[2]
    --kinevox.Log("road1 pos: "..roadPos[0].." "..roadPos[1])
  end
  road1:SetPosition(roadPos[0], math.floor(roadPos[1]+0.5), roadPos[2])
  
  
  local road2 = kinevox.GetActor("Road2")
  local roadPos = road2:GetPosition()
  roadPos[1] = roadPos[1] + velocityY
  if roadPos[1] > HALF_SCREEN_HEIGHT + SCREEN_SIZE[1] then
    roadPos[0] = HALF_SCREEN_WIDTH
    roadPos[1] = HALF_SCREEN_HEIGHT - SCREEN_SIZE[1]
    roadPos[2] = roadPos[2]
    --kinevox.Log("road2 pos: "..roadPos[0].." "..roadPos[1])
  end
  road2:SetPosition(roadPos[0], math.floor(roadPos[1]+0.5), roadPos[2])
  
end

function MinesLogic(dt)
  if START_WIDTH>END_WIDTH then
    START_WIDTH = START_WIDTH - 1
  end
  local velocityY = VELOCITY[2] * dt
  for i=1,MINE_COUNT do
    local leftMine = kinevox.GetActor("LMine"..i)
    --MineLogic(leftMine, dt)
    local lpos = leftMine:GetPosition()
    
    local lnewY = lpos[1]+velocityY
    local lnewX = lpos[0]
    if lnewY > SCREEN_SIZE[1]+16 then
      lnewY = -16
      local movement = 8*math.random(-1, 1)
      TOP_MINE_POS[1] = TOP_MINE_POS[1] + movement
      lnewX = TOP_MINE_POS[1] - START_WIDTH
    end
    leftMine:SetPosition(lnewX, math.floor(lnewY+0.5), lpos[2])
    
    local rightMine = kinevox.GetActor("RMine"..i)
    --MineLogic(rightMine, dt)
    local rpos = rightMine:GetPosition()
    local rnewY = rpos[1]+velocityY
    local rnewX = rpos[0]
    if rnewY > SCREEN_SIZE[1]+16 then
      rnewY = -16
      rnewX =  TOP_MINE_POS[1] + START_WIDTH
    end
    rightMine:SetPosition(rnewX,  math.floor(rnewY+0.5), rpos[2])
  end
    
end


function CarLogic(dt)
    --Test collision against paddle
    local car = kinevox.GetActor("Car")
    local carPos = car:GetPosition()
end

function Logic(self, dt)
  --kinevox.Log("dt: "..dt.." Velocity: "..VELOCITY[1].." "..VELOCITY[2]) 
  RoadLogic(dt)
  MinesLogic(dt)
  CarLogic(dt)
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