import * as React from 'react';
import {useState} from 'react';
import {
  View,
  Text,
  TouchableOpacity,
  NativeModules,
  ScrollView,
} from 'react-native';

// import TurboModule from 'rtn-calculator/js/NativeCalculator';

const {OldCalculatorModule, AdapterJSIModule} = NativeModules;

let installed = false;

if (installed) {
  AdapterJSIModule.install();
  installed = true;
}

const makeNetworkCall = async (
  type: 'turbo' | 'native',
): Promise<{
  type: 'turbo' | 'native';
  roundTrip: number;
}> => {
  const start = Date.now();
  let response = null;
  if (type === 'native') {
    response = await OldCalculatorModule.get_string_via_bridge();
  } else {
    response = makeNetworkCall_get_writable_map();
  }
  const end = Date.now();
  if (__DEV__) {
    console.log(JSON.stringify(response));
  }
  return {
    type,
    roundTrip: end - start,
  };
};

let aggregateData: {
  avgTurboTime: number;
  numberOfTurboTrip: number;
  avgNativeTime: number;
  numberOfnativeTrip: number;
} = {
  avgTurboTime: 0,
  numberOfTurboTrip: 0,
  avgNativeTime: 0,
  numberOfnativeTrip: 0,
};

const __temp = Array.from({length: 1}, (_, index) => index);

const App = () => {
  // :::  this crashes
  // React.useEffect(() => {
  //   const _init_native_lib = async () => {
  //     await AdapterJSIModule.install();
  //   };
  //   _init_native_lib();
  // }, []);

  const [responses, setResponses] = useState<
    {
      type: 'turbo' | 'native';
      roundTrip: number;
    }[]
  >([]);

  const appendResponse = (response: {
    type: 'turbo' | 'native';
    roundTrip: number;
  }) => {
    if (response.type === 'turbo') {
      const newTurboTrips = aggregateData.numberOfTurboTrip + 1;
      const newTurboAvg =
        (aggregateData.avgTurboTime * aggregateData.numberOfTurboTrip +
          response.roundTrip) /
        newTurboTrips;

      aggregateData = {
        ...aggregateData,
        avgTurboTime: newTurboAvg,
        numberOfTurboTrip: newTurboTrips,
      };
    } else {
      const newNativeTrips = aggregateData.numberOfnativeTrip + 1;
      const newNativeAvg =
        (aggregateData.avgNativeTime * aggregateData.numberOfnativeTrip +
          response.roundTrip) /
        newNativeTrips;

      aggregateData = {
        ...aggregateData,
        avgNativeTime: newNativeAvg,
        numberOfnativeTrip: newNativeTrips,
      };
    }
    setResponses(prevResponse => [...prevResponse, response]);
  };

  const performExperiment = async (type: 'native' | 'turbo') => {
    for (const __index of __temp) {
      const newReponse = await makeNetworkCall(type);
      appendResponse(newReponse);
    }
  };

  return (
    <View
      style={{
        flex: 1,
        gap: 16,
        padding: 16,
        paddingBottom: 64,
      }}>
      <View
        style={{
          flexDirection: 'row',
          justifyContent: 'center',
          alignItems: 'center',
          gap: 16,
          paddingVertical: 32,
        }}>
        <TouchableOpacity
          onPress={async () => {
            await AdapterJSIModule.install();
            await performExperiment('turbo');
          }}
          style={{
            padding: 16,
            backgroundColor: '#83fa7d',
          }}>
          <Text>Custom JSI Bindings</Text>
        </TouchableOpacity>

        <TouchableOpacity
          onPress={async () => {
            await performExperiment('native');
          }}
          style={{
            padding: 16,
            backgroundColor: '#42b3f5',
          }}>
          <Text>Native Module</Text>
        </TouchableOpacity>
      </View>

      <View>
        <Text
          style={{
            fontSize: 16,
            fontWeight: 'bold',
            backgroundColor: '#83fa7d',
            padding: 4,
          }}>
          Avg Turbo: {aggregateData.avgTurboTime} | trips:{' '}
          {aggregateData.numberOfTurboTrip}
        </Text>
        <View style={{height: 24}} />
        <Text
          style={{
            fontSize: 16,
            fontWeight: 'bold',
            backgroundColor: '#42b3f5',
            padding: 4,
          }}>
          Avg Native: {aggregateData.avgNativeTime} | trips:{' '}
          {aggregateData.numberOfnativeTrip}
        </Text>
      </View>

      <ScrollView>
        {responses.map(({type, roundTrip}, index) => (
          <View
            style={{
              backgroundColor: type === 'turbo' ? '#83fa7d' : '#42b3f5',
              padding: 8,
              flexDirection: 'row',
              marginVertical: 2,
            }}
            key={`roundTrip-${index}`}>
            <Text>
              {type.toUpperCase()} {roundTrip}
            </Text>
          </View>
        ))}
      </ScrollView>
    </View>
  );
};

export default App;
