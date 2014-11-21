function Initialize(self)
  kinevox.Log("TestInit")
  local inspect = kinevox.LoadLib('inspect')  
  --print(inspect.inspect(self))
  s = 1
end

function TestKinevoxModule(self)
  --Test Log
  s = s+s
  kinevox.Log("TestLog"..s)
  
    --Test GetScreen size
  SCREEN_SIZE = kinevox.GetScreenSize()
  kinevox.Log("TestInit"..SCREEN_SIZE[0].." "..SCREEN_SIZE[1].." ") --..KINEVOX_ACTOR_ID)
  assert(SCREEN_SIZE[0]==800)
  assert(SCREEN_SIZE[1]==600)
  
  --test GetActor and Get/SetPositoin
  scriptedActor2 = kinevox.GetActor("TestScriptedActor2")
  scriptedActor2:SetPosition(444, 555, 666)
  scriptedActorPos = scriptedActor2:GetPosition()
  
  kinevox.Log("ScriptedAcotr position: "..scriptedActorPos[0].." "..scriptedActorPos[1].." "..scriptedActorPos[2])
  assert(scriptedActorPos[0] == 444 and scriptedActorPos[1] == 555 and scriptedActorPos[2] == 666)
  
  --Test calling remote function
  scriptedActor2:RemoteFunction()
end

function TestObjectModule(self)
  kinevox.Log("Actor Name: "..self:GetName())
  --Test Get/Set Object
  self:SetPosition(10,20)
  pos = self:GetPosition()
  assert(pos[0] == 10 and pos[1] == 20)
  kinevox.Log("position: "..pos[0].." "..pos[1].." "..pos[2])
  
  self:SetVisible(false)
  assert(self:IsVisible() == false)
  self:SetVisible(true)
  assert(self:IsVisible() == true)
end

function TestInputModule(self)
  --Test IsKeyDown
  local isKeyDown = false
  if input.IsKeyDown(GCL_RIGHT) then
    isKeyDown = true
  end
end

function TestSoundModule(self)
  --Test PlaySound
  sound.PlaySound("Explosion.wav")
end

function Logic(self, dt)
  self:TestKinevoxModule()
  self:TestObjectModule()
  self:TestInputModule()
  self:TestSoundModule()
end

function RemoteFunction(self)
  kinevox.Log("Calling Remote function from actor"..self:GetName())
  assert(self:GetName() == "TestScriptedActor2")
end

function Terminate(self)
  kinevox.Log("TestTerminate")
end