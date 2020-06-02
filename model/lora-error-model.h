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
 *         Andrea Sacco <andrea.sacco85@gmail.com>
 *         To Thanh Hai <tthhai@gmail.com>
 */


#ifndef LORA_ERROR_MODEL_H
#define LORA_ERROR_MODEL_H

#include <ns3/object.h>

namespace ns3 {

	/**
	 *
	 * Model for the bit errors in noisy/AWGN channels for LoRa.
	 */
	class LoRaErrorModel : public Object
	{
		public:
			/**
			 * Get the type ID.
			 */
			static TypeId GetTypeId (void);

			LoRaErrorModel (void);

			/**
			 * Return BER for given SNR.
			 */
			long double GetBER2 (double snr, uint16_t spreading,int bandwidth) const;

	};


} // namespace ns3

#endif /* LORA_ERROR_MODEL_H */


