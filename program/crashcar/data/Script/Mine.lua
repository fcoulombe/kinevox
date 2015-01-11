function Initialize(self)
    SCREEN_SIZE = kinevox.GetScreenSize()
    VELOCITY = { -0.0,50.0} 
end

function Logic(self, dt)
    --kinevox.Log("dt: "..dt.." Velocity: "..VELOCITY[1].." "..VELOCITY[2]) 
    pos = self:GetPosition()
   
    local newX = pos[0]
    local newY = pos[1]    
     
    local velocityX = VELOCITY[1] * dt
    local velocityY = VELOCITY[2] * dt
    newX = newX + velocityX 
    newY = newY + velocityY 
    
    if newY > SCREEN_SIZE[1]+32+1 then
      newY = -32
    end
    
    
    --Test collision against paddle
    local car = kinevox.GetActor("Car")
    local carPos = car:GetPosition()
    
    self:SetPosition(newX, newY, pos[2])
end


function Terminate(self)
end
