mkdir build
cd build
cmake ../
make

./geoloader ../data/Step1_Box_Pixel_Brl1926A_BeamExtension.db
./geoloader ../data/Step2_Tube_Pixel_Brl1720A_PST_Stiffner.db
./geoloader ../data/Step3_Tubs_Muon_JNSH_tx1stm_element3.db
./geoloader ../data/Step4_Pcon_LAr_Endcap_Cryostat_Cone_Mixed.db
 
