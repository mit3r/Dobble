import Dots from "@/Components/Dots";
import { CommunicationStatus, ConnectionError, ConnectionStatus, GameStatus } from "@/types/dobble";
import type { JSX } from "react";

export function GameStatusSpan({ status }: { status: GameStatus }): JSX.Element {
  switch (status) {
    case GameStatus.Waiting:
      return <span className="text-dobble-accent">Waiting</span>;
    case GameStatus.InGame:
      return <span className="text-dobble-success">In Progress</span>;
    case GameStatus.Finished:
      return <span className="text-dobble-text-muted">Finished</span>;
    default:
      return <span className="text-dobble-text-muted">Unknown</span>;
  }
}

export function ConnectionStatusSpan({ status }: { status: ConnectionStatus }): JSX.Element {
  switch (status) {
    case ConnectionStatus.Connected:
      return <span className="text-dobble-success">Connected</span>;
    case ConnectionStatus.Connecting:
      return <span className="text-dobble-accent"><Dots text="Connecting" /></span>;
    case ConnectionStatus.Disconnected:
      return <span className="text-dobble-danger">Disconnected</span>;
    case ConnectionStatus.HostLookup:
      return <span className="text-dobble-accent"><Dots text="Looking up host" /></span>;
    case ConnectionStatus.Retrying:
      return <span className="text-dobble-accent"><Dots text="Reconnecting" /></span>;
    default:
      return <span className="text-dobble-text-muted">Unknown</span>;
  }
}

export function ConnectionErrorSpan({ error }: { error: ConnectionError }): JSX.Element {
  switch (error) {
    case ConnectionError.None:
      return <span className="text-dobble-success">No Error</span>;
    case ConnectionError.NetworkError:
      return <span className="text-dobble-danger">Network Error</span>;
    case ConnectionError.Timeout:
      return <span className="text-dobble-danger">Connection Timeout</span>;
    case ConnectionError.ConnectionRefused:
      return <span className="text-dobble-danger">Connection Refused</span>;
    case ConnectionError.HostNotFound:
      return <span className="text-dobble-danger">Host Not Found</span>;
    default:
      return <span className="text-dobble-danger">Unknown Error</span>;
  }
}

export function CommunicationStatusSpan({ status }: { status: CommunicationStatus }): JSX.Element {
  switch (status) {
    case CommunicationStatus.None:
      return <span className="text-dobble-text-muted">No Communication</span>;
    case CommunicationStatus.Good:
      return <span className="text-dobble-success">Good</span>;
    case CommunicationStatus.Waiting:
      return <span className="text-dobble-accent"><Dots text="Waiting" /></span>;
    case CommunicationStatus.Retrying:
      return <span className="text-dobble-accent"><Dots text="Retrying" /></span>;
    case CommunicationStatus.Failed:
      return <span className="text-dobble-danger">Failed</span>;
    default:
      return <span className="text-dobble-text-muted">Unknown</span>;
  }
}
