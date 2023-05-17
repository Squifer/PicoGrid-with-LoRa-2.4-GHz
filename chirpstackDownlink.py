# Source: https://www.chirpstack.io/docs/chirpstack/api/python-examples.html?highlight=Downlink#enqueue-downlink
import os
import sys
from time import sleep

import grpc
from chirpstack_api import api
from influxdb import InfluxDBClient
import influxdb_client
from influxdb_client.client.write_api import SYNCHRONOUS
# Configuration.

# This must point to the API interface.
server = "192.168.1.76:8080"

# The DevEUI for which you want to enqueue the downlink.
dev_eui1 = "70B3D58FF0040150"

# The API token (retrieved using the web-interface).
api_token1 = "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJhdWQiOiJjaG" \
                           "lycHN0YWNrIiwiaXNzIjoiY2hpcnBzdGFjayIsInN1YiI6IjEzN" \
                           "DMzNWVkLWMwYTMtNDQ4Ny1iZjRiLTZiYTE1NTU3ZjllYyIsInR" \
                           "5cCI6ImtleSJ9.mBNIpfeD7kQ1cVU9pmCUzNtQAUKjeopbMihYL_EjUm4"

bucket = "chirpstack"
org = "Project"
token = "hcNv6G9csIfAvSpQB1Rt3hAnUOjf4CJttLAlIoctLi_zqvz_jKWhgQgbMEYXFlxSLyeiwOXaFhLSOnC1LKXFgg=="
# Store the URL of your InfluxDB instance
url="http://localhost:8086"
if __name__ == "__main__":
  # Connect without using TLS.
  channel = grpc.insecure_channel(server)

  # Device-queue API client.
  client = api.DeviceServiceStub(channel)

  # Define the API key meta-data.
  auth_token1 = [("authorization", "Bearer %s" % api_token1)]

  # Construct request for device 1 (smart meter).
  req = api.EnqueueDeviceQueueItemRequest()
  req.queue_item.confirmed = False
  req.queue_item.data = bytes([0x01, 0x02, 0x03])
  req.queue_item.dev_eui = dev_eui1
  req.queue_item.f_port = 10

  # connect to InfluxDB
  influxClient = influxdb_client.InfluxDBClient(
      url=url,
      token=token,
      org=org
  )
  query_api = influxClient.query_api()

  # Construct request for device 2 (inverter).
  #req = api.EnqueueDeviceQueueItemRequest()
  #req.queue_item.confirmed = False
  #req.queue_item.data = bytes([0x01, 0x02, 0x03])
  #req.queue_item.dev_eui = dev_eui2
  #req.queue_item.f_port = 10

  while True:
      # send downlink transmission to smart meter
      resp = client.Enqueue(req, metadata=auth_token1)
      # Print the downlink id
      print(resp.id)

      # create continuous query for influx
      query = 'from(bucket:"chirpstack") ' \
              '|> range(start: -30s)'
      result = query_api.query(org=org, query=query)
      results = []
      for table in result:
          for record in table.records:
              results.append((record.get_field(), record.get_value()))

      print(results)

      # send downlink transmission to inverter
      #resp = client.Enqueue(req, metadata=auth_token1)
      # Print the downlink id
      #print(resp.id)
      sleep(30)
