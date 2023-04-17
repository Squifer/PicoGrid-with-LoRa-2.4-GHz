# Source: https://www.chirpstack.io/docs/chirpstack/api/python-examples.html?highlight=Downlink#enqueue-downlink
import os
import sys

import grpc
from chirpstack_api import api
# Configuration.

# This must point to the API interface.
server = "192.168.1.76:8080"

# The DevEUI for which you want to enqueue the downlink.
dev_eui = "70B3D58FF0040150"

# The API token (retrieved using the web-interface).
api_token = "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJhdWQiOiJjaG" \
                           "lycHN0YWNrIiwiaXNzIjoiY2hpcnBzdGFjayIsInN1YiI6IjEzN" \
                           "DMzNWVkLWMwYTMtNDQ4Ny1iZjRiLTZiYTE1NTU3ZjllYyIsInR" \
                           "5cCI6ImtleSJ9.mBNIpfeD7kQ1cVU9pmCUzNtQAUKjeopbMihYL_EjUm4"

if __name__ == "__main__":
  # Connect without using TLS.
  channel = grpc.insecure_channel(server)

  # Device-queue API client.
  client = api.DeviceServiceStub(channel)

  # Define the API key meta-data.
  auth_token = [("authorization", "Bearer %s" % api_token)]

  # Construct request.
  req = api.EnqueueDeviceQueueItemRequest()
  req.queue_item.confirmed = False
  req.queue_item.data = bytes([0x01, 0x02, 0x03])
  req.queue_item.dev_eui = dev_eui
  req.queue_item.f_port = 10

  resp = client.Enqueue(req, metadata=auth_token)

  # Print the downlink id
  print(resp.id)
