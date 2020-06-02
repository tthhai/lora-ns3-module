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


#ifndef LORA_PHY_GW_H
#define LORA_PHY_GW_H

#include "lora-phy.h"
#include <ns3/object.h>
#include <ns3/nstime.h>
#include <ns3/spectrum-channel.h>
#include <ns3/spectrum-phy.h>
#include "lora-error-model.h"
#include "lora-spectrum-signal-parameters.h"
#include <ns3/event-id.h>
#include <ns3/random-variable-stream.h>
#include <ns3/callback.h>
namespace ns3 {

	class SpectrumChannel;
	class SpectrumValue;
	class SpectrumModel;
	class NetDevice;
	struct SpectrumSignalParameters;

	class LoRaPhyGw : public LoRaPhy 
	{

		public:
			LoRaPhyGw ();
			~LoRaPhyGw ();

			void DoDispose (void);

			static TypeId GetTypeId (void);
		uint32_t GetReceptions ();
		void StartRx (Ptr<SpectrumSignalParameters> paras);

		/**
		 * Verify if packet is correctly received. 
		 */
		void EndRx (Ptr<LoRaSpectrumSignalParameters> paras);
		/**
		 * Start the transmission of the given packet.
		 */
		void SetReceptionEndCallback(Callback<void,Ptr<Packet>,uint32_t,uint8_t,uint32_t,double> callback);
		/**
		 * Get the number of collisions that happened at receptions.
		 */
		uint32_t GetCollisions (void); 

		private:
		uint32_t m_collisions;
		std::vector <Ptr<LoRaSpectrumSignalParameters> > m_params;
		Callback<void, Ptr<Packet>,uint32_t, uint8_t, uint32_t,double> m_ReceptionEnd;
	};
} // namespace ns3

#endif /* LORA_PHY_GW_H */
