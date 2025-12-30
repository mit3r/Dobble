import { ConnectionStatus } from "@/types/dobble";


export function getHumanConnectionStatus(status: ConnectionStatus): string {
  switch (status) {
    case ConnectionStatus.Connecting:
      return "Connecting...";
    case ConnectionStatus.Retrying:
      return "Retrying...";
    case ConnectionStatus.Disconnected:
      return "Disconnected";
    default:
      return "Unknown status";
  }
}