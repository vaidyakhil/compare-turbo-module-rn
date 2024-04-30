import {TurboModule, TurboModuleRegistry} from 'react-native';

export interface Spec extends TurboModule {
  add(a: number, b: number): Promise<number>;

  makeNetworkCall(requestBody: Object): Promise<Object>;

  makeNetworkCall_without_promise(requestBody: Object): Object;
}

export default TurboModuleRegistry.get<Spec>('RTNCalculator') as Spec | null;
