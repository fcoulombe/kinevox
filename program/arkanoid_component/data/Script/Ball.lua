function Initialize()
    SCREEN_SIZE = kinevox.GetScreenSize()
    VELOCITY = { -6.0,-3.0} 
    kinevox.Log("CreateBall " .. SCREEN_SIZE[0] .. " " .. SCREEN_SIZE[1])
end

function Logic()
    pos = object.GetPosition()

    local newX = pos[0]
    local newY = pos[1]
    if pos[0] > SCREEN_SIZE[0] then
      VELOCITY[0] = VELOCITY[0]*-1.0
      newX = (Real)viewPort.GetWidth();
    elseif pos[0] < 0.0 then
      VELOCITY[0] = VELOCITY[0]*-1.0
      newX = 0.0
    end
    
    if pos[1] < 0.0 then
      VELOCITY[1] = VELOCITY[1]*-1.0
      newY = 0.0
    elseif pos[1] > SCREEN_SIZE[1] then
      mVelocity[0] = mVelocity[0]*-1.0
      mVelocity[1] = mVelocity[1]*-1.0
      --newPosition = START_POSITION;
      newX = 0
      newY = 0
    end
    object.SetPosition(newX, newY, 0)
    
end


function Terminate()
    kinevox.Log("Destroy ball")
end
