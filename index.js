/**
 * @format
 */

import MessageQueue from 'react-native/Libraries/BatchedBridge/MessageQueue.js';


/**
 * replace impl of spy method with the following to reduce noise
**/
// static spy(spyOrToggle: boolean | ((data: SpyData) => void)) {
//     if (spyOrToggle === true) {
//       MessageQueue.prototype.__spy = info => {

//         if (info.module != null && info.module != 'WebSocketModule' && info.module != 'BugsnagReactNative') {
//           console.log(
//             `${info.type === TO_JS ? 'N->JS' : 'JS->N'} : ` +
//               `${info.module != null ? info.module + '.' : ''}${info.method}`,
//           );
//         }
        
//       };
//     } else if (spyOrToggle === false) {
//       MessageQueue.prototype.__spy = null;
//     } else {
//       MessageQueue.prototype.__spy = spyOrToggle;
//     }
//   }
MessageQueue.spy(true);

import {AppRegistry} from 'react-native';
import App from './App';
import {name as appName} from './app.json';

AppRegistry.registerComponent(appName, () => App);
