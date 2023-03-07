#!/usr/bin/bash
departments="
animation
cfx
comp
fx
lighting
matchmove
"
for department in $departments;do

id_list=$(cat ./id_data_$department)
for id in $id_list;do
    mkdir -p /Rnd/sg_global/.tmp/data_log/$department/$id
    chmod -R 777 /Rnd/sg_global/.tmp/data_log/$department/$id
    cat > test/idleTimeChk_$id.desktop << EOF
[Desktop Entry]
Type=Application
Exec=/Rnd/sg_global/.tmp/idleTimeChk 15 /Rnd/sg_global/.tmp/data_log/$department/$id $id.log
Hidden=false
NoDisplay=false
X-GNOME-Autostart-enabled=true
Name[en_US]=idle time checker
Name=idle time checker
Comment[en_US]=
Comment=
EOF
    sshpass -p pass\!123 ssh -o StrictHostKeyChecking=no $id@$id mkdir /home/$id/.config/autostart
    sshpass -p pass\!123 scp -o StrictHostKeyChecking=no test/idleTimeChk_$id.desktop $id@$id:/home/$id/.config/autostart/
    rm -f test/idleTimeChk_$id.desktop

done
done