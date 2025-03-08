#include "components/ungoogled/fingerprinting/protections/webrtc_protection.h"

namespace ungoogled {

void WebRTCProtection::ProtectPeerConnection(RTCPeerConnection* connection) {
  if (!config_.protect_peer_connection) {
    return;
  }

  RTCConfiguration rtc_config;
  
  // Force relay-only mode
  if (config_.force_relay_only) {
    rtc_config.iceTransportPolicy = "relay";
  }
  
  // Disable MDNS
  if (config_.disable_mdns) {
    rtc_config.enableMdns = false;
  }
  
  // Configure allowed STUN/TURN servers
  if (config_.protect_ice_servers) {
    rtc_config.iceServers.clear();
    for (const auto& server : config_.allowed_stun_servers) {
      rtc_config.iceServers.push_back({server});
    }
    for (const auto& server : config_.allowed_turn_servers) {
      rtc_config.iceServers.push_back({server});
    }
  }
  
  connection->setConfiguration(rtc_config);
}

void WebRTCProtection::FilterICECandidates(std::vector<RTCIceCandidate>* candidates) {
  if (!config_.hide_local_candidates) {
    return;
  }

  // Remove local network candidates
  candidates->erase(
    std::remove_if(candidates->begin(), candidates->end(),
                   [](const RTCIceCandidate& candidate) {
                     return IsLocalCandidate(candidate);
                   }),
    candidates->end());
}

std::string WebRTCProtection::StandardizeSDPOffer(const std::string& sdp) {
  if (!config_.standardize_sdp) {
    return sdp;
  }

  // Remove fingerprinting information from SDP
  std::string filtered_sdp = sdp;
  filtered_sdp = RemoveSDPFingerprints(filtered_sdp);
  filtered_sdp = StandardizeICEParameters(filtered_sdp);
  filtered_sdp = NormalizeMediaDescriptions(filtered_sdp);
  
  return filtered_sdp;
}

} // namespace ungoogled