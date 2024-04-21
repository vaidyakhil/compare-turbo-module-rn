/**
 * @format
 */

if (__DEV__) {
  const MessageQueue = require('react-native/Libraries/BatchedBridge/MessageQueue');
  MessageQueue.spy(true);
}

import {AppRegistry} from 'react-native';
import App from './App';
import {name as appName} from './app.json';

AppRegistry.registerComponent(appName, () => App);
