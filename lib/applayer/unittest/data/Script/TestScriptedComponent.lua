
function Initialize()
    SCREEN_WIDTH = kinevox.GetScreenWidth()
    SCREEN_HEIGHT  = kinevox.GetScreenHeight()
    assert(SCREEN_WIDTH==640)
    assert(SCREEN_HEIGHT==480)
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