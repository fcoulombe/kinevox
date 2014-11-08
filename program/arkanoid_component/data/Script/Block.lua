function Initialize(self)
end

function Logic(self, dt)
end

function Hit(self)
  kinevox.Log("hit!")
  sound.PlaySound("explosion.wav")
  self:SetVisible(false)
end

function Terminate(self)
end
