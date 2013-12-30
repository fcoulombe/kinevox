
function Initialize()
    kinevox.Log("TestInit")
    s = 1
end

function Logic()
    s = s+s
    kinevox.Log("TestLog"..s)
end


function Terminate()
    kinevox.Log("TestTerminate")
end