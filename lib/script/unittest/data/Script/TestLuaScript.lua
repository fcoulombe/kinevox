bleh = 0
function Logic()
    bleh = bleh+1
    print("Logic "..bleh.."!\n")
end

function Logic2(floatArg)
    assert(floatArg == 1.0)
     print("float arg: "..floatArg.."!\n")
end

