/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 * Author: Leonard Tracy <lentracy@gmail.com>
 *         To Thanh Hai <tthhai@gmail.com>
 */


#include "lora-phy-gw.h"
#include "lora-phy.h"
#include "lora-spectrum-signal-parameters.h"
#include "lora-phy-header.h"
#include <ns3/object.h>
#include <ns3/spectrum-phy.h>
#include <ns3/net-device.h>
#include <ns3/mobility-model.h>
#include <ns3/spectrum-value.h>
#include <ns3/spectrum-channel.h>
#include <ns3/log.h>
#include <ns3/spectrum-model.h>
#include <ns3/antenna-model.h>
#include <ns3/simulator.h>
#include <ns3/event-id.h>
#include <ns3/random-variable-stream.h>
#include <ns3/double.h>
#include <cmath>
namespace ns3 {

	NS_LOG_COMPONENT_DEFINE ("LoRaPhyGw");

	NS_OBJECT_ENSURE_REGISTERED (LoRaPhyGw);


	TypeId
		LoRaPhyGw::GetTypeId (void)
		{
			static TypeId tid = TypeId ("ns3::LoRaPhyGw")
				.SetParent<LoRaPhy> ()
				.AddConstructor<LoRaPhyGw> ()
				;
			return tid;
		}

	LoRaPhyGw::LoRaPhyGw ()
		:LoRaPhy()
	{
		NS_LOG_FUNCTION (this);
		m_collisions = 0;
	}

	LoRaPhyGw::~LoRaPhyGw ()
	{
		NS_LOG_FUNCTION (this);
	}

	void 
		LoRaPhyGw::DoDispose (void)
		{
		m_netDevice = 0;
		m_mobility = 0;
		LoRaPhy::DoDispose ();
		}

	void
		LoRaPhyGw::SetReceptionEndCallback (Callback<void,Ptr<Packet>,uint32_t,uint8_t,uint32_t,double> callback)
		{
			NS_LOG_FUNCTION (this);
			m_ReceptionEnd = callback;
		}


	void
		LoRaPhyGw::StartRx (Ptr<SpectrumSignalParameters> paras)
		{
			NS_LOG_FUNCTION (this << paras);
			Ptr<LoRaSpectrumSignalParameters> sfparas = DynamicCast<LoRaSpectrumSignalParameters> (paras);
			*m_receivingPower += *paras->psd;
			Simulator::Schedule(paras->duration,&LoRaPhyGw::EndNoise,this,paras->psd);
			m_ReceptionStart();
			if (sfparas != 0){
				Simulator::Schedule(sfparas->duration,&LoRaPhyGw::EndRx,this,sfparas);
				uint32_t channel = sfparas->GetChannel();
				uint32_t bandwidth = sfparas->GetBandwidth();
				for (auto &i : m_paras)
				{
					if (2*i->GetBandwidth()/pow(2,i->GetSpreading()-2) == 2*sfparas->GetBandwidth()/pow(2,sfparas->GetSpreading()-2))
					{
						if ( i->GetChannel()+i->GetBandwidth()/200> channel-bandwidth/200 && i->GetChannel()-i->GetBandwidth()/200 < channel+bandwidth/200)
						{
							m_collisions++;

							double Poweri = 0.0;
							double Powerj = 0.0;
							for (uint8_t p = 0; p<25; p++){
								Poweri += (*i->psd)[p];
								Powerj += (*sfparas->psd)[p];
							}
							if (Poweri > 4*Powerj)
							{
								sfparas->SetBer(10);
							}
							else
							{
								if (Powerj > 4*Poweri)
								{
									i->SetBer(10);
								}
								else
								{
									m_collisions++;
									i->SetBer(10);
									sfparas->SetBer(10);
								}
							}
						}
					}
				}

				if (GetReceptions () > 8)
				{
					sfparas->SetBer (10);
				}

				m_paras.push_back(sfparas);
			}
			else
			{
			}
		}

	uint32_t
		LoRaPhyGw::GetCollisions()
		{
			return m_collisions;
		}

	uint32_t 
		LoRaPhyGw::GetReceptions()
		{
			uint8_t temp = 0;
			for (auto &i : m_paras)
			{
				if(i->GetBer()==0)
				{
					temp++;
				}
			}
			return temp;
		}

	void 
		LoRaPhyGw::EndRx (Ptr<LoRaSpectrumSignalParameters> paras)
		{
			NS_LOG_FUNCTION(this << paras);
			std::vector<Ptr<LoRaSpectrumSignalParameters> >::iterator temp=m_paras.begin();
			for (std::vector<Ptr<LoRaSpectrumSignalParameters> >::iterator it=m_paras.begin(); it!=m_paras.end();it++)
			{
				if (*it==paras)
				{
					temp = it;
					break;
				}
			}
			m_paras.erase(temp);
			NS_LOG_DEBUG("paras are deleted" << paras << GetReceptions());

			if(paras->GetBer()<5)
			{

				LoRaPhyHeader lh;
				Ptr<Packet> packet = paras->packet;
				packet->RemoveHeader(lh);
				m_ReceptionEnd( packet, paras->GetBandwidth(), paras->GetSpreading(), paras->GetChannel(),(*paras->psd)[(paras->GetChannel()-868e4)/250+1]*paras->GetBandwidth());
			}
			else
			{

				m_ReceptionError();
			}
		}



} // namespace
