rm -Rf ~/autoJenkins/klog
cd ~/autoJenkins/trunk
svn update
cd ~/autoJenkins/util
svn update macOS-localDeployFromJenkins.sh
cp -R ~/autoJenkins/trunk ~/autoJenkins/klog
cp ~/autoJenkins/util/macOS-localDeployFromJenkins.sh ~/autoJenkins/klog/
cd ~/autoJenkins/klog
chmod 700 ~/autoJenkins/klog/macOS-localDeployFromJenkins.sh
~/autoJenkins/klog/macOS-localDeployFromJenkins.sh
cp ~/autoJenkins/klog/*.dmg ~/autoJenkins
rm -Rf ~/autoJenkins/klog
rm -Rf ~/autoJenkins/trunk/*



