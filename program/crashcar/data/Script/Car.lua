function Initialize(self)
    SCREEN_SIZE = kinevox.GetScreenSize()
    VELOCITY = 100.0
end

function Logic(self, dt)
    pos = self:GetPosition()
    local newX = pos[0]
    local newY = pos[1]
    local yRot = 0
    if input.IsKeyDown(GCL_LEFT) then
      yRot = 3.14/6
      newX = newX - VELOCITY * dt
      if newX < 0 then
        newX = 0
      end
    end
    if input.IsKeyDown(GCL_RIGHT) then
      yRot = -3.14/6
      newX = newX + VELOCITY * dt
      if newX > SCREEN_SIZE[0] then
        newX = SCREEN_SIZE[0]
      end
    end
    self:SetOrientation(0, 0, yRot)
    self:SetPosition(newX, newY, pos[2]) 
     
end


function Terminate(self)
   
end
