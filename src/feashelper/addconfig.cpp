inline bool isNewFeas()
{
    if(Testfile("/sys/module/mtk_fpsgo/parameters/target_fps_61"))
    {
        return true;
    }
    if(Testfile("/sys/module/perfmgr/parameters/target_fps_61"))
    {
        return true;
    }
    return false;
}

