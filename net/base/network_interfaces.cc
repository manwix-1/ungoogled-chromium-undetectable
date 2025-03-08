bool GetNetworkList(NetworkInterfaceList* networks, int policy) {
  auto* instance_manager = ungoogled::InstanceManager::GetInstance();
  
  // Check if this is an automated instance
  if (auto* instance = instance_manager->GetCurrentInstance()) {
    // Use instance-specific network configuration
    return instance->GetNetworkStack()->GetNetworkList(networks, policy);
  }
  
  // Default behavior for normal usage
  return GetDefaultNetworkList(networks, policy);
}
