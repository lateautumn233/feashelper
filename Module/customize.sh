SKIPUNZIP=0
MODDIR=${0%/*}
alias sh='/system/bin/sh'
echo "----------------------------------------------------"
# print FEATURES slowly
cat $MODPATH/FEATURES | while read row; do echo "$row";sleep 0.1;done;echo ""

echo "Please wait…"
rm $MODPATH/FEATURES

# no uperf
[ $(pgrep uperf) == "" ] && echo "Uperf detected, please remove." && abort

# remove old version
MODS_PATH="/data/adb/modules"
[ -d $MODS_PATH/Feashelper_Mtk ] && rm -rf $MODS_PATH/Feashelper_Mtk

# no miui13
[ $(getprop ro.miui.ui.version.code) -lt 14 ] && echo "Unsupported MIUI version detected, please upgrade." && abort

# nfc fix
if [ -f /product/pangu ]; then
    mkdir -p $MODPATH/system/product/
    cp -r /product/pangu/system/* $MODPATH/system/product/
fi

# keep config
[ ! -f /data/feas.conf ] && cp $MODPATH/feas.conf /data/feas.conf
# mv /data/feas.conf /data/feas.conf.bak
# cp -f $MODPATH/feas.conf /data/feas.conf
# echo "Please note that this update does not preserve the configuration."

# remove model config
rm $MODPATH/feas.conf

# permission
chmod a+x $MODPATH/FEAShelper

# start FEAShelper on install
pkill -9 FEAShelper
echo "----------------------------------------------------"
$MODPATH/FEAShelper /data/feas.conf &
sleep 1s

# test if FEAShelper started
if [[ "$(pgrep FEAShelper)" == "" ]]; then
    echo "Sorry, unsupported device."
    abort
fi
echo "FEAShelper is running……"