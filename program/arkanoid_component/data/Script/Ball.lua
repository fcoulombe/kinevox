function Initialize()
    SCREEN_SIZE = kinevox.GetScreenSize()
    VELOCITY = { -6.0,-3.0} 
    kinevox.Log("CreateBall " .. SCREEN_SIZE[0] .. " " .. SCREEN_SIZE[1] .. " Velocity: " .. VELOCITY[1] .. VELOCITY[2])
end

function Logic()
   
    pos = object.GetPosition()
   
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
     
    kinevox.Log("CreateBall " .. newX .. " " .. newY .. " Velocity: " .. VELOCITY[1] .. VELOCITY[2])
    local velocityX = VELOCITY[1]
    local velocityY = VELOCITY[2]
    newX = newX + velocityX
    newY = newY + velocityY
     kinevox.Log("ball logic: " .. pos[0] .. " " .. pos[1] .. " -> " .. newX .. " " .. newY)
    object.SetPosition(newX, newY, 0)
    
end


function Terminate()
    kinevox.Log("Destroy ball")
end
