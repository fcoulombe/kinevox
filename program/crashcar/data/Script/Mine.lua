function Initialize(self)
    SCREEN_SIZE = kinevox.GetScreenSize()
    VELOCITY = { -0.0,50.0}
    START_WIDTH = 120
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
    
    if newY > SCREEN_SIZE[1]+16 then
      newY = -16
      newX = newX + math.random(-1*16, 1*16)
    end
    
    
    --Test collision against paddle
    local car = kinevox.GetActor("Car")
    local carPos = car:GetPosition()
    kinevox.Log("pos: "..newX.."  "..math.floor(newY+0.5).." "..pos[2]) 
    self:SetPosition(newX, math.floor(newY+0.5), pos[2])
end


function Terminate(self)
end
