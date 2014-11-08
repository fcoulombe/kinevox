function CreateBlocks()
  for i=1, NUM_ROW*NUM_COL do
    local blockName = "Block" .. i
    kinevox.Log("Creating: " .. blockName)
    kinevox.CreateActor(blockName, "Block")
    local block = kinevox.GetActor(blockName)
    
    local blockWidth = 64
    local blockHeight = 32
    local blockHalfWidth = blockWidth/2.0
    local blockHalfHeight = blockHeight/2.0

    local spaceX = (VIEWPORT_WIDTH)/NUM_COL
    local spaceY = blockHeight

    local x = ((i%NUM_COL)*spaceX) + BORDER_SIZE + blockHalfWidth
    local y = ((i/NUM_COL)*spaceY) + BORDER_SIZE + blockHalfHeight

    block:SetPosition(x, y, 0.0)
  end  
end

function CreatePaddle()
  kinevox.CreateActor("Paddle", "Paddle")
  paddle = kinevox.GetActor("Paddle")
  paddle:SetPosition(START_POSITION[1], START_POSITION[2], START_POSITION[3])
end

function CreateBall()
  kinevox.CreateActor("Ball", "Ball")
  ball = kinevox.GetActor("Ball")
  ball:SetPosition(START_POSITION[1]+100, START_POSITION[2], START_POSITION[3])
end

function Initialize(self)
  kinevox.Log("Main Game State!")
  NUM_ROW   = 5;
  NUM_COL   = 8;
  BORDER_SIZE = 0;
  BASE_X_RESOLUTION = 1024;
  BASE_Y_RESOLUTION = 768;
  SCREEN_SIZE = kinevox.GetScreenSize()
  VIEWPORT_WIDTH =  SCREEN_SIZE[0]
  VIEWPORT_HEIGHT =  SCREEN_SIZE[1]
  X_RATIO = VIEWPORT_WIDTH / BASE_X_RESOLUTION
  Y_RATIO = VIEWPORT_HEIGHT / BASE_Y_RESOLUTION
  kinevox.Log("ratio! "..X_RATIO.." "..Y_RATIO)
  
  
  START_POSITION = {640.0*X_RATIO, 700.0*Y_RATIO, 0.0}
  CreateBlocks()
  CreatePaddle()
  CreateBall()
end

function Logic(self, dt)

end

function Terminate(self)
  kinevox.DestroyActor("Ball")
  kinevox.DestroyActor("Paddle")

  for i=1, NUM_ROW*NUM_COL do
    kinevox.DestroyActor("Block" .. i)
  end
end