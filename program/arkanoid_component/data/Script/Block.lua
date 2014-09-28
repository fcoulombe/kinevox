function Initialize(self)
end

function Logic(self)
end

function Hit(self)
  kinevox.Log("hit!")
  sound.PlaySound("explosion.wav")
end

function Terminate(self)
end
