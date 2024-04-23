[#ftl]

#if ((PORT0_NB_SINKPDO+PORT0_NB_SINKAPDO) > USBPD_MAX_NB_PDO)
#error "Nb of Sink PDO/APDO is exceeding stack capabilities"
#endif
