import cantools
import glob
import regex
from functools import reduce

class Signal:
    def __init__(self, signal):
        # String descriptors
        self.name = signal.name.lower()
        self.name = ""
        Ks = []
        for token in signal.name.split("_"):
            Ls = []
            for part in regex.findall('[A-Z]?[a-z0-9]*', token):
                if part:
                    Ls += [part.lower()]
            ls = "_".join(Ls)
            if ls:
                Ks += [ls]
        self.name = "_".join(Ks)

        self.comment = signal.comment
        self.is_multiplexer = signal.is_multiplexer

        # Data descriptors
        self.is_float = signal.is_float
        self.is_signed = signal.is_signed
        self.is_float = signal.is_float
        self.endianess = signal.byte_order
        self.start = signal.start
        self.length = signal.length

        # Message descriptors
        self.maximum = signal.maximum
        self.minimum = signal.minimum
        self.offset = signal.offset
        self.scale = signal.scale
        self.multiplexer_ids = signal.multiplexer_ids
        self.receivers = signal.receivers
        self.unit = signal.unit

        if self.unit:
            if self.length <= 32:
                self.type = "double" # May regret this
            else:
                self.type = "double"
        else:
            if self.length <= 8:
                self.type = "int8_t" if self.is_signed else "uint8_t"
            elif self.length <= 16:
                self.type = "int16_t" if self.is_signed else "uint16_t"
            elif self.length <= 32:
                self.type = "int32_t" if self.is_signed else "uint32_t"
            else:
                self.type = "int64_t" if self.is_signed else "uint64_t"


class Message:
    def __init__(self, message):
        # Contained signals
        self.signals = [ Signal(signal) for signal in message.signals ]

        # String descriptors
        self.name = message.name.lower()
        self.comment = message.comment

        # Data descriptors
        self.frame_id = message.frame_id
        self.length = message.length
        self.is_extended = message.is_extended_frame
        self.multiplexer = [ signal.name for signal in self.signals if signal.is_multiplexer ]
        self.multiplexer_ids = []
        if self.multiplexer:
            self.multiplexer = self.multiplexer[0]
            self.multiplexer_ids = list(set(reduce(lambda x, y: x + y, [signal.multiplexer_ids for signal in self.signals if signal.multiplexer_ids])))
            self.multiplexer_size = max(self.multiplexer_ids)

        # Message descriptors
        self.periodicity = message.cycle_time
        self.senders = message.senders
        self.receivers = list(set(reduce(lambda x, y: x + y, list(signal.receivers for signal in self.signals) + [[]])))

class System:
    def __init__(self, ECU, system_name, database):
        self.name = system_name.lower()
        self.messages = [ Message(message) for message in database.messages]
        self.rx_messages = [ message for message in self.messages if ECU in message.receivers ]
        self.tx_messages = [ message for message in self.messages if ECU in message.senders ]


systems = []

for file_name in glob.glob("databases/*.dbc"):
    system_name = file_name[10:-4]


    systems += [System("AMS", system_name, cantools.database.load_file(file_name))]

