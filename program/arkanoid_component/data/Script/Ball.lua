function Initialize(self)
    SCREEN_SIZE = kinevox.GetScreenSize()
    VELOCITY = { -6.0,-3.0} 
    kinevox.Log("CreateBall " .. SCREEN_SIZE[0] .. " " .. SCREEN_SIZE[1] .. " Velocity: " .. VELOCITY[1] .. VELOCITY[2])
end

function Logic(self)
   
    pos = self:GetPosition()
   
    local newX = pos[0]
    local newY = pos[1]
    if newX > SCREEN_SIZE[0] then
      VELOCITY[1] = VELOCITY[1]*-1.0
      newX = SCREEN_SIZE[0]
    elseif newX < 0.0 then
      VELOCITY[1] = VELOCITY[1]*-1.0
      newX = 0.0
    end
    
    if newY < 0.0 then
      VELOCITY[2] = VELOCITY[2]*-1.0
      newY = 0.0
    elseif newY > SCREEN_SIZE[1] then
      VELOCITY[1] = VELOCITY[1]*-1.0
      VELOCITY[2] = VELOCITY[2]*-1.0
      --newPosition = START_POSITION;
      newX = 0
      newY = 0
    end
    
     
    --kinevox.Log("CreateBall " .. newX .. " " .. newY .. " Velocity: " .. VELOCITY[1] .. VELOCITY[2])
    local velocityX = VELOCITY[1]
    local velocityY = VELOCITY[2]
    newX = newX + velocityX
    newY = newY + velocityY
    
    --Test collision against the blocks
    for i=0,5*8-1,1 do 
      local tempBlock = kinevox.GetActor("Block"..i)
      local blockPos = tempBlock:GetPosition()
      if newX > blockPos[0]-32 and newX < blockPos[0]+32 and newY > blockPos[1] - 16 and newY < blockPos[1] +16 then
        VELOCITY[1] = VELOCITY[1]*-1.0
        VELOCITY[2] = VELOCITY[2]*-1.0
        newX = pos[0]
        newY = pos[1]
        tempBlock:Hit()
        break
      end
    end
    
    --Test collision against paddle
    local paddle = kinevox.GetActor("Paddle")
    local paddlePos = paddle:GetPosition()
    if newX > paddlePos[0]-64 and newX < paddlePos[0]+64 and newY > paddlePos[1] - 16 and newY < paddlePos[1] +16 then
        VELOCITY[2] = VELOCITY[2]*-1.0
        newX = pos[0]
        newY = pos[1]
    end
    
    --kinevox.Log("ball logic: " .. pos[0] .. " " .. pos[1] .. " -> " .. newX .. " " .. newY)
    self:SetPosition(newX, newY, 0)
end


function Terminate(self)
    kinevox.Log("Destroy ball")
end
