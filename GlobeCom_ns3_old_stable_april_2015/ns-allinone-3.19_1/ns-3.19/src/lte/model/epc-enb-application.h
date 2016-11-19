/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Jaume Nin <jnin@cttc.cat>
 *         Nicola Baldo <nbaldo@cttc.cat>
 */

#ifndef EPC_ENB_APPLICATION_H
#define EPC_ENB_APPLICATION_H

#include <ns3/address.h>
#include <ns3/socket.h>
#include <ns3/virtual-net-device.h>
#include <ns3/traced-callback.h>
#include <ns3/callback.h>
#include <ns3/ptr.h>
#include <ns3/object.h>
#include <ns3/lte-common.h>
#include <ns3/application.h>
#include <ns3/eps-bearer.h>
#include <ns3/epc-enb-s1-sap.h>
#include <ns3/epc-s1ap-sap.h>
#include "ns3/net-device.h"
#include <map>
#include "ns3/udp-header.h"
#include "ns3/seq-ts-header.h"
#include "ns3/tcp-header.h"
namespace ns3 {
class EpcEnbS1SapUser;
class EpcEnbS1SapProvider;


/**
 * \ingroup lte
 *
 * This application is installed inside eNBs and provides the bridge functionality for user data plane packets between the radio interface and the S1-U interface.
 */
class EpcEnbApplication : public Application
{

  friend class MemberEpcEnbS1SapProvider<EpcEnbApplication>;
  friend class MemberEpcS1apSapEnb<EpcEnbApplication>;


  // inherited from Object
public:
  static TypeId GetTypeId (void);
protected:
  void DoDispose (void);

public:
  static uint32_t onlylte;
  //static std::map<Mac48Address,std::map<double,double> > snr_per;
   
  void SendToS1uSocket (Ptr<Packet> packet, uint32_t teid);
 //  void resetfunc();
 
  void udpstats(uint32_t fid,uint32_t pid, double recvbytes, double time);
  void updatewifiload(double load);
  void updatewifisinrper(double sinr,double per, Mac48Address macaddr);
 static  void updateltesinrper(std::string context,uint16_t cellid,uint16_t rnti, double sinr, double avgsinr);
  void addbearer(Ipv4Address ip, EpsBearer bearer,double interpacketinterval);
 // static double uplinkthrpt_lte;
 

  //static double ulfirstpktsend;
  //void algo();
  /*
   * User Defined function for releasing a ue
   *
  void usrDefinedReleaseUe(uint16_t rnti);
*/

  /** 
   * Constructor
   * 
   * \param lteSocket the socket to be used to send/receive packets to/from the LTE radio interface
   * \param s1uSocket the socket to be used to send/receive packets
   * to/from the S1-U interface connected with the SGW 
   * \param enbS1uAddress the IPv4 address of the S1-U interface of this eNB
   * \param sgwS1uAddress the IPv4 address at which this eNB will be able to reach its SGW for S1-U communications
   * \param cellId the identifier of the enb
   */
  EpcEnbApplication (Ptr<Socket> lteSocket, Ptr<Socket> s1uSocket, Ipv4Address enbS1uAddress, Ipv4Address sgwS1uAddress, uint16_t cellId);
  EpcEnbApplication (void);
  /**
   * Destructor
   * 
   */
  virtual ~EpcEnbApplication (void);

/*
 * CRAN
 * A MAP to store beared ID and corresponding UE IP
 */
  std::map<uint16_t,Ipv4Address>BEARER_ID_UE_IP_MAP;
  /** 
   *
   * Set the S1 SAP User
   * 
   * \param s the S1 SAP User
   */
  void SetS1SapUser (EpcEnbS1SapUser * s);

  // WiFi netdevice


  /** 
   * 
   * \return the S1 SAP Provider
   */
  EpcEnbS1SapProvider* GetS1SapProvider ();

  /** 
   * Set the MME side of the S1-AP SAP 
   * 
   * \param s the MME side of the S1-AP SAP 
   */
  void SetS1apSapMme (EpcS1apSapMme * s);

  /** 
   * 
   * \return the ENB side of the S1-AP SAP 
   */
  EpcS1apSapEnb* GetS1apSapEnb ();
 
  /** 
   * Method to be assigned to the recv callback of the LTE socket. It is called when the eNB receives a data packet from the radio interface that is to be forwarded to the SGW.
   * 
   * \param socket pointer to the LTE socket
   */
  void RecvFromLteSocket (Ptr<Socket> socket);


  /** 
   * Method to be assigned to the recv callback of the S1-U socket. It is called when the eNB receives a data packet from the SGW that is to be forwarded to the UE.
   * 
   * \param socket pointer to the S1-U socket
   */
  void RecvFromS1uSocket (Ptr<Socket> socket);
  void addusedrbs(double usedrbs);

  struct EpsFlowId_t
  {
    uint16_t  m_rnti;
    uint8_t   m_bid;

  public:
    EpsFlowId_t ();
    EpsFlowId_t (const uint16_t a, const uint8_t b);

    friend bool operator == (const EpsFlowId_t &a, const EpsFlowId_t &b);
    friend bool operator < (const EpsFlowId_t &a, const EpsFlowId_t &b);
  };
struct time_info{
	uint32_t source_port;
	int64_t past;
	SequenceNumber32 last_seqno;
	uint32_t dest_port;
	SequenceNumber32 window_size;
};
struct pkt_info {
	uint16_t source_port;
	uint16_t dest_port;
	uint32_t seqno;
	uint32_t recvbytes;
	double pktsendtime;
};
struct udp_pktinfo {
	uint32_t fid;
	uint32_t pid;
	uint32_t recvbytes;
	double pktsendtime;
};
struct flowtable_info {
        uint16_t  source_port;
        uint16_t  dest_port;
	uint32_t current_interface;
	double Qos_Enabled;
	double delay;
	double throughput;
	Ipv4Address ip;
	EpsBearer epsbearer;
	double appdatarate;
	uint32_t protocolno;
	double new_flow;
};
struct sinrperinfo {
	double lteval;
	double wifival;
};

private:

  // ENB S1 SAP provider methods
  void DoInitialUeMessage (uint64_t imsi, uint16_t rnti);
  void DoPathSwitchRequest (EpcEnbS1SapProvider::PathSwitchRequestParameters params);
  void DoUeContextRelease (uint16_t rnti);
  
  // S1-AP SAP ENB methods
  void DoInitialContextSetupRequest (uint64_t mmeUeS1Id, uint16_t enbUeS1Id, std::list<EpcS1apSapEnb::ErabToBeSetupItem> erabToBeSetupList);
  void DoPathSwitchRequestAcknowledge (uint64_t enbUeS1Id, uint64_t mmeUeS1Id, uint16_t cgi, std::list<EpcS1apSapEnb::ErabSwitchedInUplinkItem> erabToBeSwitchedInUplinkList);

  /** 
   * Send a packet to the UE via the LTE radio interface of the eNB
   * 
   * \param packet t
   * \param bid the EPS Bearer IDentifier
   */
  void SendToLteSocket (Ptr<Packet> packet, uint16_t rnti, uint8_t bid);
  void resetfunc();
   void maximizethrpt();
   void movefairly();
  void algo();
  void algoall();
  void algo_naive();
  void algo_SINR();
  void algo_LOAD();
  void algo_LOAD1();

 


 uint32_t  topsis(uint32_t gbr, uint32_t currentinterface,uint32_t rank);
 uint32_t  topsis1(uint32_t gbr, uint32_t currentinterface,uint32_t rank);
 int  GBR_lte();
 int  gbr_ensured(uint32_t current_interface);
 double  sum(uint32_t current_interface, uint32_t gbr);
 double  sum1(uint32_t current_interface);
 bool checkNGBR(uint32_t currentinterface);
 
 int nflows(uint32_t current_interface);
 Ipv4Address lessenergyue();
 Ipv4Address  moreenergyue();
 int no_gbrnotensured(uint32_t current_interface);
 int nGflows(uint32_t current_interface, double QOS);
 double nGflowstp(uint32_t current_interface, double QOS);
  /** 
   * Send a packet to the SGW via the S1-U interface
   * 
   * \param packet packet to be sent
   * \param teid the Tunnel Enpoint IDentifier
   */

 
  
  /** 
   * internal method used for the actual setup of the S1 Bearer
   * 
   * \param teid 
   * \param rnti 
   * \param bid 
   */
  void SetupS1Bearer (uint32_t teid, uint16_t rnti, uint8_t bid);

  /**
   * raw packet socket to send and receive the packets to and from the LTE radio interface
   */
  Ptr<Socket> m_lteSocket;

  /**
   * UDP socket to send and receive GTP-U the packets to and from the S1-U interface
   */
  Ptr<Socket> m_s1uSocket;

  /**
   * address of the eNB for S1-U communications
   */
  Ipv4Address m_enbS1uAddress;

  /**
   * address of the SGW which terminates all S1-U tunnels
   */
  Ipv4Address m_sgwS1uAddress;

  /**
   * map of maps telling for each RNTI and BID the corresponding  S1-U TEID
   * 
   */
  std::map<uint16_t, std::map<uint8_t, uint32_t> > m_rbidTeidMap;  

  /**
   * map telling for each S1-U TEID the corresponding RNTI,BID
   * 
   */
  std::map<uint32_t, EpsFlowId_t> m_teidRbidMap;
 
  /**
   * UDP port to be used for GTP
   */
  uint16_t m_gtpuUdpPort;

  /**
   * Provider for the S1 SAP 
   */
  EpcEnbS1SapProvider* m_s1SapProvider;

  /**
   * User for the S1 SAP 
   */
  EpcEnbS1SapUser* m_s1SapUser;

  /**
   * MME side of the S1-AP SAP
   * 
   */
  EpcS1apSapMme* m_s1apSapMme;

  /**
   * ENB side of the S1-AP SAP
   * 
   */
  EpcS1apSapEnb* m_s1apSapEnb;

  /**
   * UE context info
   * 
   */
  std::map<uint64_t, uint16_t> m_imsiRntiMap;
    /* 
     * all maps from cc file
     */   
  std::list<pkt_info> pkt_list;
std::list<udp_pktinfo> udp_pktlist;
std::map<uint16_t, std::map<uint16_t,std::map<double,uint32_t>  > >  pkttointer;
std::map<uint16_t, std::map<uint16_t,double> > lastpktrecv;
std::map<uint16_t, std::map<uint16_t,double> > firstpktsend;
std::map<uint16_t, std::map<uint16_t,double> > total_delay;
std::map<uint16_t, std::map<uint16_t,uint64_t> > RecvBytes;
std::map<uint16_t, std::map<uint16_t,double> > wlastpktrecv;
std::map<uint16_t, std::map<uint16_t,double> > wfirstpktsend;
std::map<uint16_t, std::map<uint16_t,double> > wtotal_delay;
std::map<uint16_t, std::map<uint16_t,uint64_t> > wRecvBytes; 
std::map<uint32_t, double > udp_recvbytes;
std::map<uint32_t, double > udp_sendbytes;
std::map<uint32_t, double > udp_lastpktrecv;
std::map<uint32_t, double > udp_firstpktsend;
std::map<uint32_t, double > udp_totaldelay;
std::map<uint32_t, flowtable_info> flowtable;

std::map<Ipv4Address, sinrperinfo> per_table;
std::map<Ipv4Address, EpsBearer> ipbearermap;
std::map<Ipv4Address, double> ipdatarate;

std::map<Mac48Address,double> rssi;
public:
   std::map<Mac48Address,Ipv4Address> macipmap;
private:
  uint16_t m_cellId;
  uint64_t lreset;
  double lte_usedrbs;
public:
   double uplinkthrpt_wifi;
  double uplinksendpkt;
  double uplinkthrpt_lte;
  
  double wifitpt;
  double ltetpt;
  double uplinkthrpt;
  double ullastpktrecv;
  int64_t past;
  int flag;
  int mxpt;
  int fair; 
  bool tcp;
  time_info s[10];
   uint64_t nmb;
  //uint32_t pkt_cnt=0;
  uint32_t packtno;
  uint16_t temp_port;
  uint32_t current_value;
  uint32_t past_value;
  int count;
  double  wifi_load;
};

} //namespace ns3

#endif /* EPC_ENB_APPLICATION_H */

