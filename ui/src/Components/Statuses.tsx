import Dots from "@/Components/Dots";
import { CommunicationStatus, ConnectionError, ConnectionStatus, GameStatus } from "@/types/dobble";
import type { JSX } from "react";

export function GameStatusSpan({ status }: { status: GameStatus }): JSX.Element {
  switch (status) {
    case GameStatus.Waiting:
      return <span>Waiting</span>;
    case GameStatus.InGame:
      return <span>In Progress</span>;
    case GameStatus.Finished:
      return <span>Finished</span>;
    default:
      return <span>Unknown status</span>;
  }
}

export function ConnectionStatusSpan({ status }: { status: ConnectionStatus }): JSX.Element {
  switch (status) {
    case ConnectionStatus.Connected:
      return <span>Connected</span>;
    case ConnectionStatus.Connecting:
      return <Dots text="Connecting" />;
    case ConnectionStatus.Disconnected:
      return <span>Disconnected</span>;
    case ConnectionStatus.HostLookup:
      return <Dots text="Looking up host" />;
    case ConnectionStatus.Retrying:
      return <Dots text="Reconnecting" />;
    default:
      return <span>Unknown Status</span>;
  }
}

export function ConnectionErrorSpan({ error }: { error: ConnectionError }): JSX.Element {
  switch (error) {
    case ConnectionError.None:
      return <span>No Error</span>;
    case ConnectionError.NetworkError:
      return <span>Network Error</span>;
    case ConnectionError.Timeout:
      return <span>Connection Timeout</span>;
    case ConnectionError.ConnectionRefused:
      return <span>Connection Refused</span>;
    case ConnectionError.HostNotFound:
      return <span>Host Not Found</span>;
    default:
      return <span>Unknown Error</span>;
  }
}

export function CommunicationStatusSpan({ status }: { status: CommunicationStatus }): JSX.Element {
  switch (status) {
    case CommunicationStatus.None:
      return <span>No Communication</span>;
    case CommunicationStatus.Good:
      return <span>Good</span>;
    case CommunicationStatus.Waiting:
      return <Dots text="Waiting" />;
    case CommunicationStatus.Retrying:
      return <Dots text="Retrying" />;
    case CommunicationStatus.Failed:
      return <span>Failed</span>;
    default:
      return <span>Unknown</span>;
  }
}
