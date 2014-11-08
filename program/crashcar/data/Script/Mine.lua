function Initialize(self)
    SCREEN_SIZE = kinevox.GetScreenSize()
    VELOCITY = { -0.0,1.0} 
end

function Logic(self, dt)
   
    pos = self:GetPosition()
   
    local newX = pos[0]
    local newY = pos[1]    
     
    local velocityX = VELOCITY[1]
    local velocityY = VELOCITY[2]
    newX = newX + velocityX 
    newY = newY + velocityY 
    
    if newY > SCREEN_SIZE[1] then
      newY = 0
    end
    
    
    --Test collision against paddle
    local car = kinevox.GetActor("Car")
    local carPos = car:GetPosition()
    
    self:SetPosition(newX, newY, 0)
end


function Terminate(self)
end
