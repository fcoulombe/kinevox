function Initialize(self)
    SCREEN_SIZE = kinevox.GetScreenSize()
    VELOCITY = 10.0
end

function Logic(self, dt)
    pos = self:GetPosition()
    local newX = pos[0]
    local newY = pos[1]
    if input.IsKeyDown(GCL_LEFT) then
        newX = newX - VELOCITY
        if newX < 0 then
          newX = 0
        end
    end
    if input.IsKeyDown(GCL_RIGHT) then
      newX = newX + VELOCITY
      if newX > SCREEN_SIZE[0] then
        newX = SCREEN_SIZE[0]
      end
    end
    self:SetPosition(newX, newY, 0)  
end


function Terminate(self)
   
end
