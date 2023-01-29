# !/system/bin/sh
MODDIR=${0%/*}
lock_val() {
    if [ -f "$2" ]; then
        chown root:root "$2"
        chmod 0666 "$2"
        echo "$1" >"$2"
        chmod 0444 "$2"
    fi
}

wait_until_login() {
    # in case of /data encryption is disabled
    while [ "$(getprop sys.boot_completed)" != "1" ]; do
        sleep 1
    done
    # no need to start before the user unlocks the screen
    local test_file="/sdcard/Android/.LOGIN_PERMISSION_TEST"
    true > "$test_file"
    while [ ! -f "$test_file" ]; do
        true > "$test_file"
        sleep 1
    done
    rm "$test_file"
}
fuck_dirty() {
# gpu
    lock_val "0" /sys/kernel/ged/hal/custom_upbound_gpu_freq
    lock_val "50" /sys/kernel/ged/hal/custom_boost_gpu_freq
# ged
    lock_val "0" /sys/kernel/ged/hal/fastdvfs_mode
    lock_val "0" /sys/kernel/ged/hal/fastdvfs_mode
    lock_val "1" /sys/module/ged/parameters/is_GED_KPI_enabled
# fpsgo
    lock_val "free" /sys/kernel/fpsgo/common/force_onoff
    lock_val "1" /sys/kernel/fpsgo/common/fpsgo_enable
# fpsgo dirty
    lock_val "0" /sys/kernel/fpsgo/fbt/limit_cfreq
    lock_val "0" /sys/kernel/fpsgo/fbt/limit_rfreq
    lock_val "0" /sys/kernel/fpsgo/fbt/limit_cfreq_m
    lock_val "0" /sys/kernel/fpsgo/fbt/limit_rfreq_m
    lock_val "0" /sys/module/mtk_fpsgo/parameters/boost_affinity
    lock_val "0" /sys/kernel/fpsgo/minitop/enable
# others
    lock_val "0" /sys/module/mtk_core_ctl/parameters/policy_enable
    lock_val "0" /sys/module/fbt_cpu/parameters/boost_affinity
}

stop_dirty() {
    stop vendor.miperf
    stop miuibooster
}

wait_until_login
sleep 30s
nohup $MODDIR/Feashelper /data/feas.conf >/dev/null > $MODDIR/feashelper.log &

# FUCK
stop_dirty
fuck_dirty
