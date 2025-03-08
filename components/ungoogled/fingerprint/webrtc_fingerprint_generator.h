class InfiniteWebRTCGenerator {
public:
    struct WebRTCConfig {
        struct ICE {
            std::vector<std::string> stun_servers;
            std::vector<std::string> turn_servers;
            std::vector<std::string> ice_candidates;
            bool randomize_candidate_order = true;
        };

        struct Media {
            std::vector<std::string> codecs;
            std::vector<int> bandwidth_limits;
            std::vector<std::string> device_ids;
            bool mask_real_devices = true;
        };

        struct Network {
            std::vector<std::string> local_ip_ranges;
            std::vector<std::string> protocols = {"udp", "tcp", "tls"};
            bool hide_private_ips = true;
            bool randomize_ports = true;
        };
    };

    std::string GenerateWebRTCProfile();
private:
    WebRTCConfig config_;
    std::unique_ptr<WebRTCProfileLearner> webrtc_learner_;
};