mkdir external/
cd external/
wget http://ufpr.dl.sourceforge.net/project/boost/boost/1.59.0/boost_1_59_0.tar.bz2
tar xjf boost_1_59_0.tar.bz2
mv boost_1_59_0/boost .
rm -rf boost_1_59_0
wget https://github.com/g-truc/glm/releases/download/0.9.8.3/glm-0.9.8.3.zip
unzip glm-0.9.8.3.zip
rm -rf glm-0.9.8.3.zip
mv glm glm_full
mv glm_full/glm .
rm -rf glm_full
cd ..

