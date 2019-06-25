FIntVector AWFC::MinEntropyCoords() {
	SCOPE_CYCLE_COUNTER( STAT_Entropy );

	float MinEntropy = 0;

	const FRandomStream RandomStream = FRandomStream( FMath::RandRange( 0.F, MAX_FLT ) );
	FIntVector RMinEntropyCoords = FIntVector();

	for3( OutputSize.X, OutputSize.Y, OutputSize.Z,
		  {
			  const FIntVector CurrentCoordinates = FIntVector( X, Y, Z );

			  if ( Wave.GetDataAt( CurrentCoordinates ).AllowedCount() > 1 ) {
				  const float Entropy = ShannonEntropy( CurrentCoordinates );
				  const float EntropyPlusNoise = Entropy - RandomStream.FRand();

				  if ( MinEntropy == 0 || EntropyPlusNoise < MinEntropy ) {
					  MinEntropy = EntropyPlusNoise;
					  RMinEntropyCoords = FIntVector( X, Y, Z );
				  }
			  }
		  }
	)

		return RMinEntropyCoords;
}

float AWFC::ShannonEntropy( FIntVector CurrentCoordinates ) {
	SCOPE_CYCLE_COUNTER( STAT_ShannonEntropy );

	int32 SumOfWeights = 0;
	int32 SumOfWeightsLogWeights = 0;

	for ( auto& Pair : Wave.GetDataAt( CurrentCoordinates ).AllowedPatterns ) {

		const int32 Weight = ModuleAssignee->Weights.FindRef( Pair.Key );

		SumOfWeights += Weight;
		SumOfWeightsLogWeights += Weight * static_cast< float >( FMath::Loge( Weight ) );
	}

	return FMath::Loge( SumOfWeights ) - ( SumOfWeightsLogWeights / SumOfWeights );
}
