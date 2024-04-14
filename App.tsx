import * as React from 'react';
import {View, Text, TouchableOpacity, NativeModules } from 'react-native';

import calcSync from 'rtn-calculator/js/NativeCalculator';

const { OldCalculatorModule } = NativeModules;

const App = () => {
  return (
    <View
      style={{
        justifyContent: 'center',
        alignItems: 'center',
        flex: 1,
        gap: 16
      }}>
      <TouchableOpacity
        onPress={async () => {
          const response = await calcSync.add(0, 1);
          console.log('response: ', response);
        }}
        style={{
          padding: 16,
          backgroundColor: 'green',
        }}>
        <Text>JSI Module</Text>
      </TouchableOpacity>

      <TouchableOpacity
        onPress={async () => {
          const response = await OldCalculatorModule.add(-5, 5);
          console.log('response: ', response);
        }}
        style={{
          padding: 16,
          backgroundColor: 'green',
        }}>
        <Text>Native Module</Text>
      </TouchableOpacity>
    </View>
  );
};

export default App;
