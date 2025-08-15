#!/usr/bin/env python3
"""
Simple Magic Square Pattern Visualizer
Analyzes CSV data using only standard libraries + matplotlib
"""

import csv
import matplotlib.pyplot as plt
import numpy as np
from collections import Counter, defaultdict
import argparse

class SimpleVisualizer:
    def __init__(self, csv_file='pattern_analysis.csv'):
        self.csv_file = csv_file
        self.data = []
        self.load_data()
        
    def load_data(self):
        """Load CSV data into list of dictionaries"""
        try:
            with open(self.csv_file, 'r') as f:
                reader = csv.DictReader(f)
                for row in reader:
                    # Convert numeric fields
                    row['index'] = int(row['index'])
                    row['near_miss_error'] = int(row['near_miss_error'])
                    row['ratio_range'] = float(row['ratio_range'])
                    row['equidistant_count'] = int(row['equidistant_count'])
                    
                    # Parse prime factors
                    factors = row['prime_factors'].strip().split()
                    row['factor_list'] = factors
                    row['factor_count'] = len(factors)
                    row['has_5'] = '5' in factors
                    row['has_13'] = '13' in factors
                    row['has_17'] = '17' in factors
                    row['has_41'] = '41' in factors
                    row['has_349'] = '349' in factors
                    
                    # Calculate relative error
                    row['relative_error'] = row['near_miss_error'] / row['index']
                    
                    self.data.append(row)
                    
            print(f"Loaded {len(self.data)} candidates from {self.csv_file}")
            
        except FileNotFoundError:
            print(f"Error: {self.csv_file} not found!")
        except Exception as e:
            print(f"Error loading data: {e}")
    
    def overview_stats(self):
        """Print overview statistics"""
        if not self.data:
            return
            
        print("\n" + "="*50)
        print("MAGIC SQUARE CANDIDATE ANALYSIS OVERVIEW")
        print("="*50)
        
        indices = [row['index'] for row in self.data]
        errors = [row['near_miss_error'] for row in self.data]
        rel_errors = [row['relative_error'] for row in self.data]
        
        print(f"Total candidates analyzed: {len(self.data)}")
        print(f"Index range: {min(indices):,} to {max(indices):,}")
        print(f"Best near miss error: {min(errors):,}")
        print(f"Worst near miss error: {max(errors):,}")
        
        # Best candidates by absolute error
        best_by_error = sorted(self.data, key=lambda x: x['near_miss_error'])[:5]
        print(f"\nTOP 5 BEST NEAR MISSES (absolute error):")
        for row in best_by_error:
            print(f"  {row['index']:>8}: error={row['near_miss_error']:>12,}, factors={row['prime_factors'].strip()}")
        
        # Best candidates by relative error
        best_by_rel_error = sorted(self.data, key=lambda x: x['relative_error'])[:5]
        print(f"\nTOP 5 BEST NEAR MISSES (relative error):")
        for row in best_by_rel_error:
            print(f"  {row['index']:>8}: rel_error={row['relative_error']:>10.6f}, factors={row['prime_factors'].strip()}")
        
        # Prime factor analysis
        factor_counts = {
            '5': sum(1 for row in self.data if row['has_5']),
            '13': sum(1 for row in self.data if row['has_13']),
            '17': sum(1 for row in self.data if row['has_17']),
            '41': sum(1 for row in self.data if row['has_41']),
            '349': sum(1 for row in self.data if row['has_349'])
        }
        
        total = len(self.data)
        print(f"\nPRIME FACTOR DISTRIBUTION:")
        for factor, count in factor_counts.items():
            print(f"  Has factor {factor:>3}: {count:>2}/{total} ({100*count/total:.1f}%)")
    
    def plot_error_analysis(self):
        """Plot error analysis"""
        if not self.data:
            return
            
        indices = [row['index'] for row in self.data]
        errors = [row['near_miss_error'] for row in self.data]
        rel_errors = [row['relative_error'] for row in self.data]
        
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))
        
        # Absolute error
        ax1.scatter(indices, errors, alpha=0.7, c='blue', s=20)
        ax1.set_xlabel('Index')
        ax1.set_ylabel('Near Miss Error (absolute)')
        ax1.set_title('Near Miss Error vs Index Size')
        ax1.set_yscale('log')
        ax1.grid(True, alpha=0.3)
        
        # Relative error
        ax2.scatter(indices, rel_errors, alpha=0.7, c='red', s=20)
        ax2.set_xlabel('Index')
        ax2.set_ylabel('Relative Error (error/index)')
        ax2.set_title('Relative Error vs Index Size')
        ax2.set_yscale('log')
        ax2.grid(True, alpha=0.3)
        
        plt.tight_layout()
        plt.savefig('simple_error_analysis.png', dpi=300, bbox_inches='tight')
        plt.show()
    
    def plot_factor_analysis(self):
        """Analyze prime factor patterns"""
        if not self.data:
            return
            
        factors = ['5', '13', '17', '41', '349']
        
        fig, axes = plt.subplots(2, 3, figsize=(18, 10))
        axes = axes.flatten()
        
        for i, factor in enumerate(factors):
            has_factor = [row['relative_error'] for row in self.data if factor in row['factor_list']]
            no_factor = [row['relative_error'] for row in self.data if factor not in row['factor_list']]
            
            ax = axes[i]
            if has_factor and no_factor:
                ax.hist([has_factor, no_factor], bins=20, alpha=0.7, 
                       label=[f'Has {factor}', f'No {factor}'], color=['blue', 'orange'])
                ax.set_xlabel('Relative Error')
                ax.set_ylabel('Frequency')
                ax.set_title(f'Error Distribution: Factor {factor}')
                ax.set_yscale('log')
                ax.legend()
                ax.grid(True, alpha=0.3)
        
        # Factor count distribution
        ax = axes[5]
        factor_counts = [row['factor_count'] for row in self.data]
        count_dist = Counter(factor_counts)
        
        ax.bar(count_dist.keys(), count_dist.values(), alpha=0.7, color='green')
        ax.set_xlabel('Number of Prime Factors')
        ax.set_ylabel('Count')
        ax.set_title('Distribution of Prime Factor Counts')
        ax.grid(True, alpha=0.3)
        
        plt.tight_layout()
        plt.savefig('simple_factor_analysis.png', dpi=300, bbox_inches='tight')
        plt.show()
    
    def find_patterns(self):
        """Find promising patterns"""
        if not self.data:
            return
            
        print("\n" + "="*50)
        print("PATTERN ANALYSIS")
        print("="*50)
        
        # Best by relative error
        best_relative = sorted(self.data, key=lambda x: x['relative_error'])[:10]
        print("BEST CANDIDATES BY RELATIVE ERROR:")
        for row in best_relative:
            print(f"  {row['index']:>8}: rel_error={row['relative_error']:>10.6f}, factors={row['prime_factors'].strip()}")
        
        # Factor combination analysis
        factor_patterns = []
        for row in best_relative:
            factors = tuple(sorted(row['factor_list']))
            factor_patterns.append(factors)
        
        pattern_counts = Counter(factor_patterns)
        print(f"\nMOST COMMON FACTOR PATTERNS (top 10):")
        for pattern, count in pattern_counts.most_common(5):
            print(f"  {' '.join(pattern):>15}: {count} occurrences")
        
        # Statistical insights
        ratios = [row['ratio_range'] for row in self.data]
        rel_errors = [row['relative_error'] for row in self.data]
        
        print(f"\nSTATISTICAL INSIGHTS:")
        print(f"  Mean ratio range: {np.mean(ratios):.3f}")
        print(f"  Min ratio range:  {min(ratios):.3f}")
        print(f"  Max ratio range:  {max(ratios):.3f}")
        
        # Simple correlation calculation
        correlation = np.corrcoef(ratios, rel_errors)[0, 1]
        print(f"  Ratio-Error correlation: {correlation:.3f}")
    
    def generate_large_scale_recommendations(self):
        """Generate recommendations for 1000-20M search"""
        print("\n" + "="*50)
        print("RECOMMENDATIONS FOR 1000-20MILLION SEARCH")
        print("="*50)
        
        # Find best patterns
        best_patterns = sorted(self.data, key=lambda x: x['relative_error'])[:5]
        
        print("STRATEGIC INSIGHTS:")
        print("1. Best performing factor combinations:")
        for row in best_patterns:
            factors = row['prime_factors'].strip()
            print(f"   - {factors} (index {row['index']}, rel_error={row['relative_error']:.6f})")
        
        # Analyze which factors appear most in best candidates
        top_20 = sorted(self.data, key=lambda x: x['relative_error'])[:20]
        factor_importance = defaultdict(int)
        for row in top_20:
            for factor in row['factor_list']:
                factor_importance[factor] += 1
        
        print(f"\n2. Most important factors (in top 20 candidates):")
        for factor, count in sorted(factor_importance.items(), key=lambda x: x[1], reverse=True):
            print(f"   - Factor {factor}: appears in {count}/20 ({100*count/20:.0f}%) of best candidates")
        
        # Generate some specific candidates for large-scale search
        print(f"\n3. STRATEGIC TARGETS FOR LARGE-SCALE SEARCH:")
        print("   Focus on numbers divisible by combinations of: 5, 13, 17, 41")
        print("   Target ratio ranges between 2.0 and 2.2 (based on current best performers)")
        
        best_ratios = [row['ratio_range'] for row in best_patterns]
        print(f"   Best ratio range: {min(best_ratios):.3f} to {max(best_ratios):.3f}")
    
    def run_analysis(self, show_plots=True):
        """Run complete analysis"""
        if not self.data:
            print("No data loaded!")
            return
        
        self.overview_stats()
        
        if show_plots:
            print("\nGenerating plots...")
            self.plot_error_analysis()
            self.plot_factor_analysis()
        
        self.find_patterns()
        self.generate_large_scale_recommendations()
        
        print("\n" + "="*50)
        print("ANALYSIS COMPLETE!")
        if show_plots:
            print("Generated files:")
            print("  - simple_error_analysis.png")
            print("  - simple_factor_analysis.png")
        print("="*50)

def main():
    parser = argparse.ArgumentParser(description='Simple magic square pattern analysis')
    parser.add_argument('--csv', default='pattern_analysis.csv', 
                       help='CSV file to analyze')
    parser.add_argument('--no-plots', action='store_true',
                       help='Skip generating plots')
    
    args = parser.parse_args()
    
    visualizer = SimpleVisualizer(args.csv)
    visualizer.run_analysis(show_plots=not args.no_plots)

if __name__ == "__main__":
    main()